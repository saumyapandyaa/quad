

#include "imgUtils.c"

/**
 * This is the structure we are going to use to store each individual node of
 * the BST. Remember that each Quad corresponds to a rectangular area on the
 * image:
 *
 *                (tx,ty)         w
 *                   x-------------------------
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                 h |          Quad          |
 *                   |    key = tx+(ty*sx)    |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   -------------------------x
 *                                       (tx + w, ty + h)
 *
 */
typedef struct quad
{
  int tx, ty; // The (x,y) coordinates of the top-left pixel in the quad
  int w;      // How many pixels wide the quad is
  int h;      // How many pixels high the quad is

  int sx; // Width of the original image, this is needed for the key.
          // This *MUST* be the same for all nodes in the BST

  int key; // A unique identifier (remember we discussed BST nodes
           // should have unique keys to identify each node). The
           // key identifier here will be created as:
           //       key = tx + (ty * sx)
           // This means that only one quad can start at a specific
           // pixel.

  int wsplit; // 1 if this quad is supposed to be split along the width
              // 0 if this quad is supposed to be split along the height

  /**
   * TODO: Complete the definition of the Quad struct
   */
  struct quad *right;
  struct quad *left;
} Quad;

///////////////////////////////////////////////////////////////////////////////

Quad *new_Quad(int tx, int ty, int w, int h, int wsplit, int sx)
{
  /**
   * This function creates and initializes a new Quad for a rectanglecstarting
   * at (tx, ty) with a width 'w' and height 'h'. The width ofcthe image in
   * which this rectangle exists is 'sx', use this to computecthe key as:
   *
   * 		key = tx + (ty * sx)
   *
   * TODO: Implement this function
   */
  Quad *new = (Quad *)calloc(1, sizeof(Quad));
  if (new == NULL)
  {
    return NULL;
  }
  new->tx = tx;
  new->ty = ty;
  new->w = w;
  new->h = h;
  new->sx = sx;
  new->key = (tx + (ty * sx));
  new->wsplit = wsplit;
  new->right = NULL;
  new->left = NULL;

  return new;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_insert(Quad *root, Quad *new_node)
{
  /**
   * This function inserts a new Quad node into the BST rooted atc'root'. The
   * new_node must already be initialized with validcdata, and must have a
   * unique key.
   *
   * Your function must make sure that there are no duplicate nodescwith the
   * same key in the BST, and if it finds any you shouldcprint the following
   * message to the screen:
   *
   * printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n",....);
   * (of course you need to provide the relevant variables to print)
   *
   * And it must return without inserting anyting in the BST.
   *
   * TODO: Implement this function
   */
  if (new_node == NULL)
  {
    return NULL;
  }
  if (root == NULL)
  {
    return new_node;
  }
  else
  {
    if (root->key < new_node->key)
    {
      root->right = BST_insert(root->right, new_node);
    }
    else if (root->key > new_node->key)
    {
      root->left = BST_insert(root->left, new_node);
    }
    else if (root->key == new_node->key)
    {
      printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n", root->tx, root->ty, root->sx);
    }
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_search(Quad *root, int tx, int ty)
{
  /**
   * This function searches the BST for a Quad at the speficied position. If
   * found, it must return a pointer to the quad that contains it.
   *
   * Search has to happen according to the BST search process - so you need to
   * figure out what value to use during the search process to decide which
   * branch of the tree to search next.
   *
   * Note that the 'sx' value does not need to be passed in here since it must
   * be the same as the one in any Quad already in the tree.
   *
   * Return NULL if the Quad doesn't exist in the BST.
   *
   * TODO: Implement this function
   */
  if (root == NULL)
  {
    return NULL;
  }
  int new_key = (tx + ((root->sx) * ty));
  if (root->key == new_key)
  {
    return root;
  }
  else
  {
    if (root->key < new_key)
    { // go right
      return BST_search(root->right, tx, ty);
    }
    else if (root->key > new_key)
    { // go left
      return BST_search(root->left, tx, ty);
    }
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *find_successor(Quad *right_child)
{
  /**
   * This function finds the successor of a Quad node by searching the right
   * subtree for the node that is most to the left (that will be the node
   * with the smallest key in that subtree)
   *
   * TODO: Implement this function
   */
  //       can we assume tree isnt null?
  if (right_child == NULL)
  {
    return NULL;
  }
  if (right_child->left == NULL)
  {
    return right_child;
  }
  else
  {
    return find_successor(right_child->left);
  }
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_delete(Quad *root, int tx, int ty)
{
  /**
   * Deletes from the BST a Quad at the specified position. You must implement
   * the three cases of BST deletion we discussed in class. Make sure the
   * function can remove a Quad at any position without breaking the tree!
   *
   * Once again, remember that 'sx' is stored in the tree.
   *
   * TODO: Implement this function
   */

  if (root == NULL)
  {
    return NULL;
  }
  int new_key = (tx + ((root->sx) * ty));
  // if root key == new key --> root node
  // if "sub" root key < new key --> "sub" roots
  if (root->key > new_key)
  {
    root->left = BST_delete(root->left, tx, ty);
  }
  // if "sub" root key > new key --> "sub" roots
  else if (root->key < new_key)
  {
    root->right = BST_delete(root->right, tx, ty);
  }
  // if "sub" root key == new key --> "sub" roots
  else if (root->key == new_key)
  {
    Quad *new_root = root;
    // 0 children
    if (root->right == NULL && root->left == NULL)
    {
      free(root);
      return NULL;
    }
    // 1 child, 2 cases
    else if (root->right == NULL)
    {
      root = root->left;
      free(new_root);
    }
    else if (root->left == NULL)
    {
      root = root->right;
      free(new_root);
    }
    // 2 children
    else
    {
      /*new_root=find_successor(root->right);
      new_root->right=root->right;
      new_root->left=root->left;
      free(root);
      root=new_root;*/
      new_root = find_successor(root->right);
      root->key = new_root->key;
      root->tx = new_root->tx;
      root->ty = new_root->ty;
      root->w = new_root->w;
      root->h = new_root->h;
      root->sx = new_root->sx;
      root->wsplit = new_root->wsplit;
      root->right = BST_delete(root->right, new_root->tx, new_root->ty);
    }
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *delete_BST(Quad *root)
{
  /**
   * This function deletes the BST and frees all memory used for nodes in it.
   * Recall that there is a specific order in which this needs to be done!
   * (consult the Unit 4 notes as needed)
   *
   * This function should return NULL.
   *
   * TODO: Implement this function
   */
  if (root == NULL)
  {
    return NULL;
  }
  if (root->left != NULL)
  {
    delete_BST(root->left);
  }
  if (root->right != NULL)
  {
    delete_BST(root->right);
  }
  free(root);
  // if(root->right==NULL && root->left == NULL){
  // printf("deleted RS n LS.");
  //}
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BST_inorder(Quad *root, int depth)
{
  /**
   * This function performs an in-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you
   * print, you can see at what level each node is located! (this should help
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */
  if (root == NULL)
  {
    return;
  }
  if (root->left != NULL)
  {
    depth++;
    BST_inorder(root->left, depth);
    // depth++;
  }
  // depth++;
  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx,
         root->ty, root->w, root->h, root->wsplit);
  if (root->right != NULL)
  {
    depth++;
    BST_inorder(root->right, depth);
  }
  // depth++;
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_preorder(Quad *root, int depth)
{
  /**
   * This function performs a pre-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you
   * print, you can see at what level each node is located! (this should help
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root == NULL)
  {
    return;
  }

  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx,
         root->ty, root->w, root->h, root->wsplit);
  if (root->left != NULL)
  {
    depth++;
    BST_preorder(root->left, depth);
    // depth++;
  }
  if (root->right != NULL)
  {
    depth++;
    BST_preorder(root->right, depth);
    // depth++;
  }
  // depth++;
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_postorder(Quad *root, int depth)
{
  /**
   * This function performs a post-order traversal of the BST and prints out
   * the information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you
   * print, you can see at what level each node is located! (this should help
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */
  if (root == NULL)
  {
    return;
  }
  // int d=depth;
  if (root->left != NULL)
  {
    depth++;
    BST_postorder(root->left, depth);
  }
  if (root->right != NULL)
  {
    // depth++;
    depth++;
    BST_postorder(root->right, depth);
  }
  // depth++;
  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx,
         root->ty, root->w, root->h, root->wsplit);
  // depth++;

  return;
}

///////////////////////////////////////////////////////////////////////////////
// NOTE: For the remaining functions, you may assume the following:          //
//       (1) All the Quads are valid (None of them go outside the image)     //
//       (2) They don't overlap  (A pixel will not be in multiple Quads)     //
///////////////////////////////////////////////////////////////////////////////

int get_colour(Image *im, Quad *q)
{
  /**
   * Given an image 'im' and a Quad 'q', get the colour we should be assigning
   * to the pixels that are in it, and return it. For the sake of this
   * assignment, we will say this is *average* colour of all the pixels in
   * the quad.
   *
   * The pixel data is stored in a one dimensional array called 'data' in the
   * image struct. Make sure you look at the definition of this to understand
   * how the image is stored. Remember that the pixel data is stored in
   * row-major order, so to get the colour for pixel (x,y) you will look at the
   * index
   *
   *                           x + (y * sx)
   *
   * of the array.
   *
   * TODO: Implement this function. You should not be getting any values
   *       outside the range of the pixels [0-255] if you have implemented
   *       this correctly.
   */
  // tests to do-
  // if data is empty
  // if quad==NULL
  // if image == NULL

  if (im == NULL)
  {
    return -1;
  }
  if (q == NULL)
  {
    return -1;
  }
  int sum = 0, avg = 0, index = 0, count = 0;
  for (int i = 0; i < (q->h); i++)
  {
    for (int j = 0; j < (q->w); j++)
    {
      int x = q->tx + j;
      int y = q->ty + i;
      int z = q->sx;
      index = (x + (y * z));
      sum += im->data[index];
      count++;
    }
  }
  // printf("get_colour: sum=%d\n",sum);
  // printf("get_colour: count=%d\n",count);
  avg = sum / count;
  // printf("get_colour: average=%d\n",avg);
  return avg;
}

///////////////////////////////////////////////////////////////////////////////

int similar(Image *im, Quad *q, int threshold)
{
  /**
   * Given an image 'im', check if the colours in the area corresponding to the
   * Quad 'q' are all similar. If not, we will have to split it. For the
   * purpose of this assigment, we say the colours in a Quad are similar if
   *
   *          maxCol - minCol <= threshold
   *
   * where maxCol and minCol are the maximum and minimum values respectively
   * of the pixel colours in the Quad. The threshold is a parameter. This
   * function should return a 0 if the pixels are not similar enough and the
   * Quad needs to be split, and 1 otherwise.
   *
   * TODO: Implement this function
   */
  if (im == NULL)
  {
    return -1;
  }
  if (q == NULL)
  {
    return -1;
  }
  int z = q->sx;
  int x = q->tx;
  int y = q->ty;
  int index = 0, minCol = 5000, maxCol = -71;
  for (int i = 0; i < (q->h); i++)
  {
    for (int j = 0; j < (q->w); j++)
    {
      x = q->tx + j;
      y = q->ty + i;
      index = (x + (y * z));
      // printf("data[index]= %d\n",im->data[index]);
      if (minCol > im->data[index])
      {
        minCol = im->data[index];
      }
      if (maxCol < im->data[index])
      {
        maxCol = im->data[index];
      }
    }
  }
  // printf("min: %d\n",minCol );
  // printf("max: %d\n",maxCol );
  // printf("threshold= %d\n", threshold);
  // printf("maxCol-minCol= %d\n", maxCol-minCol);
  if (maxCol - minCol <= threshold)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

///////////////////////////////////////////////////////////////////////////////

Quad *split_tree(Image *im, Quad *root, int threshold)
{
  /**
   * This function traverses the BST, and for each existing Quad, checks if
   * the pixels in the quad are of a similarcolour using the similar() function
   * and the given threshold. If they are not, then the Quad needs to be split
   * into 2 Quads(which will hopefully have pixels that are more similar to
   * each other).
   *
   * To do this, first we need to decide in which direction we are going to
   * split the Quad. For this, we will use the 'wsplit'field.
   *
   *    - If wsplit = 1, then we split it along the width (ie, we will now have
   *                     2 quads with the same heightand half the width as the
   *                     original one)
   *
   *    - If wsplit = 0, then we split along the height.
   *
   * NOTE: We don't always want to split the Quads in the same direction
   *       every time this function is called, because then we could just
   *       end up with very thin and long/tall quads, which wouldn't be very
   *       helpful to what we are trying to do. So, we need to make sure that
   *       once we split a Quad, that we invert the value of the 'wsplit'
   *       variable in both the new nodes, so they split the other way.
   *
   * -------------------------------------------------------------------------
   *
   * For example, if our Quad had the following values:
   *    (tx:ty = 0:0     w = 512,   h = 512,  wsplit = 1) ---> A
   *
   *                (0,0)
   *                   x-------------------------
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |           A            |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   -------------------------x
   *                                         (512, 512)
   *
   *                                     * this pixel is not IN the image, just
   *                                       represents the 'corner'. The bottom
   *                                       right pixel, as always, is (511,511)
   *
   * it would be split along the width, and the resulting two Quads
   * we would get would be as follows:
   *
   *     (tx:ty =  0 :0     w = 256,   h = 512,  wsplit = 0) ---> B
   *     (tx:ty = 256:0     w = 256,   h = 512,  wsplit = 0) ---> C
   *
   *
   *                (0,0)       (256, 0)
   *                   x-----------x-------------
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |     B     |      C     |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   -------------------------x
   *                                         (512, 512)
   *
   *   - Note that we want to always split it in exactly half, but if the
   *     width/height is an odd number then round down.
   *
   *   - Further note that 'wsplit' on both of these has now been set to 0.
   *     If they were split again, the resulting Quads would have wsplit = 1.
   *
   * --------------------------------------------------------------------------
   *
   * Now, once you know how it needs to be split, carefully form these two
   * Quads, with the correct positions and sizes, and replace the the original
   * one with them.
   *
   * This function is crunchy - and if you don't think it through before you
   * start implementing it you'll run into all kinds of trouble.
   *
   * This is the problem solving exercise for A2, so don't look for people
   * on Piazza to give you answers, or tell you what to do, or verify you're
   * doing the right thing.
   *
   * It's up to you how to solve this, and if you want an opinion, you can
   * come to visit us during office hours! The included file `point.pgm` is
   * a good candidate image to test this function on.
   *
   * Expected result: The BST will have at most twice as many Quads
   *                  as before, depending on how many of them needed to be
   *                  split.
   *
   * TODO: Implement this function
   */
  if (root == NULL || im == NULL)
  {
    return NULL;
  }

  root->right = split_tree(im, root->right, threshold);
  // printf("1st tx:ty = %d:%d     w = %d,   h = %d,  wsplit = %d\n", root->tx, root->ty, root->w, root->h,root->wsplit);
  int is_similar = similar(im, root, threshold);
  if (is_similar == 0)
  {
    // printf("is_similar=0\n");
    // wanna split
    if (root->wsplit == 1)
    {
      // printf("wsplit=1\n");
      // split along width(same height, w=w/2)
      root->w = root->w / 2;
      root->wsplit = 0;
      Quad *new = NULL;
      new = new_Quad(root->w + root->tx, root->ty, root->w, root->h, root->wsplit, root->sx);
      root = BST_insert(root, new);
    }
    else if (root->wsplit == 0)
    {
      // printf("wsplit=0\n");
      // split along height(same width, h=h/2)
      root->h = root->h / 2;
      root->wsplit = 1;
      Quad *new = NULL;
      new = new_Quad(root->tx, root->ty + root->h, root->w, root->h, root->wsplit, root->sx);
      root = BST_insert(root, new);
    }
    // printf("2nd tx:ty = %d:%d     w = %d,   h = %d,  wsplit = %d\n", root->tx, root->ty, root->w, root->h,root->wsplit);
  }
  /* else if(is_similar==1){
      // printf("is_similar=1\n");
   //don't split
   return root;
 }  */
  // printf("3rd tx:ty = %d:%d     w = %d,   h = %d,  wsplit = %d\n", root->tx, root->ty, root->w, root->h,root->wsplit);
  root->left = split_tree(im, root->left, threshold);
  return root;
}
///////////////////////////////////////////////////////////////////////////////

void drawOutline(Image *im, Quad *root, unsigned char col)
{
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each quad
   * and draw an outline for it. The outline consists of the outermost pixels
   * of the Quad (ie, the top and bottom rows, and the leftmost and rightmost
   * columns).
   *
   * Make sure that these outlines are of the input colour 'col' that is passed
   * in. The colour of the remaining pixels should not be changed.
   *
   * TODO: Implement this function
   */
  if (root == NULL || im == NULL)
  {
    return;
  }
  drawOutline(im, root->left, col);
  drawOutline(im, root->right, col);
  int index = 0, z = root->sx, y = 0, x = 0;
  // printf("h: %d, w: %d\n", root->h+1, root->w);
  // top most row
  for (int i = 0; i < (root->w); i++)
  {
    x = root->tx + i;
    y = root->ty;
    index = (x + (y * z));
    im->data[index] = col;
    // printf("drawOutline: top %d\n ", im->data[index]	);
  }
  // printf("\n");
  // bottom most row
  for (int i = 0; i < (root->w); i++)
  {
    x = root->tx + i;
    y = root->ty + (root->h - 1);
    index = (x + (y * z));
    im->data[index] = col;
    // printf("drawOutline: bottom %d\n", im->data[index]);
  }
  // printf("\n");
  // right most col
  for (int i = 0; i < (root->h); i++)
  {
    y = root->ty + i;
    x = root->tx + (root->w - 1);
    index = (x + (y * z));
    im->data[index] = col;
    //  printf("drawOutline: right %d\n", im->data[index]);
  }
  // printf("\n");
  // left most col
  for (int i = 0; i < (root->h); i++)
  {
    y = root->ty + i;
    x = root->tx;
    index = (x + (y * z));
    im->data[index] = col;
    // printf("drawOutline: left %d\n", im->data[index]);
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////

void save_Quad(Image *im, Quad *root)
{
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each
   * quad, and set all the pixels in the corresponding area to the expected
   * colour of the quad computed by your function get_colour().
   *
   * Make sure you index into the pixels array correctly and change the colour
   * in the image itself.
   *
   * TODO: Implement this function
   */
  if (root == NULL || im == NULL)
  {
    return;
  }
  int index = 0;
  int colour = get_colour(im, root);
  //  printf("h=%d, w=%d\n",root->h,root->w);
  //  printf("colour: %d\n",colour);
  for (int i = 0; i < (root->h); i++)
  {
    for (int j = 0; j < (root->w); j++)
    {
      int x = root->tx + j;
      int y = root->ty + i;
      int z = root->sx;
      index = (x + (y * z));
      im->data[index] = colour;
      //   printf("(%d,%d) :%d\n",x,y,colour);
    }
  }
  save_Quad(im, root->left);
  save_Quad(im, root->right);
  return;
}
///////////////////////////////////////////////////////////////////////////////
// That's it, you're done!
