I=imread('.\pictures\Color_120\Color_120_0.jpg');
I1=I(:,:,1);
I2=I(:,:,2);
I3=I(:,:,3);

[y,x,z]=size(I);


d1=zeros(y,x);
d2=d1;
myI=double(I);
I0=zeros(y,x);
for i=1:x
    for j=1:y
        d1(j,i)=sqrt((myI(j,i,1)-150)^2+(myI(j,i,2)-150)^2+(myI(j,i,3)-150)^2);   % Euler距离计算
        d2(j,i)=sqrt((myI(j,i,1)-20)^2+(myI(j,i,2)-20)^2+(myI(j,i,3)-20)^2);        
        if (d1(j,i)>=d2(j,i))
             I0(j,i)=1;          % 颜色聚类
        end 
    end 
end
J=I0;
tic
SE=strel('rectangle', [20 15]);      %  形态学“闭”运算
IM=imclose(J,SE);
t=toc

figure(1);imshow(I);
figure(2);subplot(1,3,1);imhist(I1);title('R');
subplot(1,3,2);imhist(I2);title('G');
subplot(1,3,3);imhist(I3);title('B');
figure(4);imshow(I0);
figure,imshow(IM);
