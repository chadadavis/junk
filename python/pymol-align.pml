
# Related:
# From: http://sourceforge.net/mailarchive/message.php?msg_id=26751711
# See also: transformations.py at http://www.lfd.uci.edu/~gohlke/

# align the pieces
super vl,vh
super cl,ch

python

# get the transformation matrices
mat_v = cmd.get_object_matrix("vl")
mat_c = cmd.get_object_matrix("cl")

# turns Pymol's output into a 4x4 python array
def mat_to_R(mat):
    R = [[mat[0],mat[1],mat[2],mat[3]],
         [mat[4],mat[5],mat[6],mat[7]],
         [mat[8],mat[9],mat[10],mat[11]],
         [mat[12],mat[13],mat[14],mat[15]]]
    return R

Rv = mat_to_R(mat_v)
Rc = mat_to_R(mat_c)


