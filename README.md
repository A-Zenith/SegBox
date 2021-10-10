This is code for the Segmentation Box project.

The project aims to make a box which is able to automatically segment out the main object in an enclosed box.

The setting is as shown in the picture:![IMG_7853](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7853.JPG)

We first simply use background subtraction to get the region of interest of the object:

![IMG_7854](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7854.JPG)

![IMG_7855](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7855.PNG)


![IMG_7855](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7856.PNG)

We then deploy the saliency detection model from *Dynamic Feature Integration for Simultaneous Detection of Salient Object, Edge and Skeleton, Jiang-Jiang Liu, Qibin Hou, Ming-Ming Cheng. **IEEE TIP**, 2020* to eventually obtain the mask of the object and segment it out.



![IMG_7869](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7869.PNG)

![IMG_7870](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7870.PNG)

![IMG_7867](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7867.PNG)

![IMG_7868](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7868.PNG)

![IMG_7866](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7866.PNG)

![IMG_7865](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7865.PNG)

![IMG_7863](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7863.PNG)

![IMG_7864](https://github.com/A-Zenith/SegBox/blob/main/imgs/IMG_7864.PNG)

