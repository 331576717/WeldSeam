function a=getPoints(X)
    figure(1);imshow(X);
    im = rgb2gray(X);
    im = im2bw(im);
    se = strel('line',5,0);
    fo = imopen(im,se);
    se = strel('line',5,90);
    fo = imopen(fo,se);
    figure(2);imshow(fo);
    foc = imclose(fo,se);
    se = strel('line',5,0);
    foc = imclose(foc,se);
    figure(3);imshow(foc);
     %foc = bwmorph(foc,'shrink');
    %figure(4);imshow(foc);
    a=foc;
   end