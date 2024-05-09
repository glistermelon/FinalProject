## Stubs to be implemented

### `Block::get_triangulation()`
Don't implement this (I've already started on it). However, **you have to understand what this function does** because it makes implementing all the other functions much easier. This function returns a list of triangles which make up the triangle decomposition of the block, as in the image below. Triangulation **has already been implemented** as part of the drawing process, but minor modifications need to be made to get the results from that process out of this function.

![triangulation image](README/triangulation.png)

### `Block::moment_of_inertia()`
I haven't looked into this enough to know *for certain* if it is actually possible to implement given our constraints, but as the name implies, this should return the moment of inertia of the block. This will involve using Calculus to, from what I can tell, integrate over point masses within the block. I would use `Block::get_triangulation()` to get those point masses, since it is easy to discern whether or not a point is within a triangle.

### `Block::bounding_box()`
Returns an (unrotated) rectangle which perfectly contains the block. Unlike `Block::center_of_mass()`, the rotation and position of the block must be taken into account.

### `Triangle::centroid()`
Returns the centroid of the triangle.

### `QuadtreeNode::are_colliding(Block* b1, Block* b2);`
Returns `true` if the two blocks are overlapping each-other (colliding), or `false` otherwise. This should involve three phases:
* Check if both blocks are present in that node of the quadtree (the `blocks` field). If they are not both present, then they are in different, non-overlapping regions, and cannot be colliding.
* If that check passes, next, check for possible collision using `Block::bounding_box()`. If the bounding boxes of the shapes do not overlap, then they are not colliding.
* Lastly, call `Block::get_triangulation()` on both blocks, then iterate over all the triangles composing each block. If any of the triangles composing one block intersects a triangle composing the other block, then the blocks must be colliding.

## Completed (to be tested)

### `Block::center_of_mass()`
Returns the location of the block's center of mass **relative to the first Point in the vertices field, assuming that the object's rotation is zero**. The easiest way to implement this is *probably* by using `Block::get_triangulation()` to get the list of triangles that make up the block, then calling `Triangle::centroid()` on each triangle (the centroid of a triangle is its center of mass) and using that information to calculate the center of mass of the block itself.

### `Vect2`
Comes with the following functions:
* `direction()`
* `magnitude()`
* (static) `dot_product(Vect2, Vect2)`
* operators for multiplication and division by scalar values
* operators for addition and subtraction of other `Vect2`'s

This is just a vector consisting of two doubles, which are usually going to be x and y coordinates, thus the `direction()` and `magnitude()` functions should return the direction and magnitude of the vector assuming that the vector's first value is an x coordinate and the vector's second value is a y coordinate.

For specifics:
* `direction` returns the angle between the vector and the +x axis, as if the vector was placed on the unit circle. Basically, it returns the theta seen in the image below. Make sure you use `normalize_angle`, defined in `Block.h`, on your output to ensure that it falls between 0 and 2*pi radians.
    ![vector image](README/unit%20circle%20vector.png)
* `magnitude` returns the length of the vector.