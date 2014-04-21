function im = test(I)
%    w = fspecial('gaussian',[15 15],0.1);
    r = I(:, :, 1);
    g = I(:, :, 2);
    b = I(:, :, 3);
    % x = rgb2hsv(I);imshow(x);imshow(x(:,:,1)>0.6 & x(:,:,2)>0.3);
%     rf = imfilter(r, w, 'replicate');
%     gf = imfilter(g, w, 'replicate');
%     bf = imfilter(b, w, 'replicate');
     h1 = fspecial('gaussian');
    rf = imfilter(double(r), h1);
    gf = imfilter(double(g), h1);
    bf = imfilter(double(b), h1);
%    
%    
    X(:,:,1) = uint8(rf);
    X(:,:,2) = uint8(gf);
    X(:,:,3) = uint8(bf);
    figure(1),imshow(X);
    imgray = rgb2gray(X);
     imgray = imadjust(imgray,[0.0,0.7],[0.0,0.7]);
    figure(2),imshow(imgray);
%       %imgray = medfilt2(imgray,[5]);
     %imgray = histeq(imgray);
     %figure(11),imshow(imgray);
%     tmin = min(imgray(:));
%     tmax = max(imgray(:));
%     th = (tmin+tmax)/2;
%     ok = true;
%     while ok
%         g1 = imgray >= th;
%         g2 = imgray < th;
%         u1 = mean(imgray(g1));
%         u2 = mean(imgray(g2));
%         thnew = (u1+u2)/2;
%         ok = abs(th - thnew) >=1;
%         th = thnew;
%     end
%     th = floor(th);
% %     imgray = imadjust(imgray,[0.0,0.7],[0.0,1]);
% %     imgray = histeq(imgray);
% %     figure(3),imshow(imgray);
%     imbw = edge(imgray,'canny');
%     figure(4),imshow(imbw);
%     im = imbw;
%     [M,N,Z] = size(I);
%     X = zeros(M,N);
%     for i = 1:M
%         for j=1:N
%             if(rf(i,j)>60 && rf(i,j) <110 && gf(i,j) > 90 && gf(i,j) <160 && bf(i,j) > 90 && bf(i,j) < 150)
%                 X(i,j) = 1;
%             else
%                 X(i,j) = 0;
%             end
%         end
%     end
%     imshow(X);
%     im = X;
%     mask = roipoly(I);
%     red = immultiply(mask,I(:, :, 1));
%     green = immultiply(mask,I(:, :, 2));
%     blue = immultiply(mask,I(:, :, 3));
%     g = cat(3, red, green, blue);
%     [M, N, K] = size(g);
%     in = reshape(g, M * N, 3);
%     idx = find(mask);
%     in = double(in(idx, 1:3));
%     [C, m] = covmatrix(in);
%     d = diag(C);
%     sd = sqrt(d)';
%     im = colorseg('euclidean', I, 25, m);
    %imshow(im);
    %imGray=rgb2gray(I);
    %imshow(imGray);
    imbw = edge(imgray,'prewitt');
    imbw2 = bwmorph(imbw,'clean',Inf);
    figure(3),imshow(imbw);
    imbw2 = bwmorph(imbw,'clean',Inf);
    %se = strel('disk',2);
   %imbw2 = bwmorph(imbw,'skel',Inf);
   %imbw2 = bwmorph(imbw,'clean',Inf);
%     [imbw2,bd]=my_bandtrace(imbw);
%     im = imbw2;
     figure(4),imshow(imbw2);
     im=imbw;
    %figure(5),imshow(imbw2-imbw);
end