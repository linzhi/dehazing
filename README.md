Single Image Dehazing
======================

Single image dehazing using dark channel prior. more information you can see at [Single Image Haze Removal Using Dark Channel Prior] [1].

Implement soft matting with the help of `boost::ublas` and `boost numeric bindings`, but the speed is not fast and can't handle large pictures. I consider to reconstruct more matrix manipulation code with Eigen.

The result you can see at `result/` folder. `refine_t.png` represents the refine transmission image by [soft matting] [3].

[1]:http://research.microsoft.com/en-us/um/people/jiansun/papers/dehaze_cvpr2009.pdf
[2]:http://research.microsoft.com/en-us/um/people/kahe/cvpr09/cvpr09slides.pdf
[3]:http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=4359322


