function [row,col]=getLocation(X)
     %w=[-1 -1 -1;-1 8 -1;-1 -1 -1]; g = abs(imfilter(double(X),w));T = max(g(:));g = g >= T;figure(5);imshow(g);imwrite(g,'res.jpg');
     m = 12;
     n = 12;
     g = imsubtract(ordfilt2(X,m*n,ones(m,n)),ordfilt2(X,1,ones(m,n)));
     g=im2bw(g);
     g=bwmorph(g,'shrink');
     imwrite((1-g),'res.jpg');
     [row,col] = find(g==1);
end