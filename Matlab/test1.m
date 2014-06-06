function b=test1() 
I=imread('Color_120_1.jpg');
%imgray = I(:,:,1)*0.7 + I(:,:,2)*0.2 + I(:,:,3)*0.1;
imgray=rgb2gray(I);
%figure(),imshow(I);
w=fspecial('gaussian',[10 10],0.5);
imgray = imfilter(imgray,w,'replicate');
figure(),imshow(imgray);
imgray = medfilt2(imgray,[6 6]);
figure(),imshow(imgray);
se=strel('rectangle',[20 10]);
g=imsubtract(imadd(imgray,imtophat(imgray,se)),imbothat(imgray,se));
figure(),imshow(g);
%imhist(g)

figure(),imshow(g>30);
figure,imshow(g);
bw=logical(g<50);
bw(:,1000:end)=0;
bw(750:end,:)=0;
figure,imshow(bw);
[L, num]=bwlabeln(bw,4);

maxIndex=[0,0];
secondMaxIndex=[0,0];
for i=1:num
    tempMax=size(find(L==i),1);
    if(tempMax>secondMaxIndex(1,1))
        secondMaxIndex(1,2)=i;
        secondMaxIndex(1,1)=tempMax;
        if(secondMaxIndex>maxIndex)
            swap = maxIndex;
            maxIndex = secondMaxIndex;
            secondMaxIndex = swap;
        end
    end
end

%figure(),imshow(L==maxIndex(1,2) | L==secondMaxIndex(1,2));

[x y]=find(L==maxIndex(1,2));
maxx0=mean(x),maxy0=mean(y)
alpha=0:pi/200:2*pi;%角度[0,2*pi]
R=10;%半径
x=round(maxx0+R*cos(alpha));
y=round(maxy0+R*sin(alpha));
for i=1:size(x,2)
    I(x(1,i),y(1,i),1)=255;
    I(x(1,i),y(1,i),2)=0;
    I(x(1,i),y(1,i),3)=0;
end

[x y]=find(L==secondMaxIndex(1,2));
secondMaxx0=mean(x),secondMaxy0=mean(y)
alpha=0:pi/200:2*pi;%角度[0,2*pi]
R=10;%半径
x=round(secondMaxx0+R*cos(alpha));
y=round(secondMaxy0+R*sin(alpha));
for i=1:size(x,2)
    I(x(1,i),y(1,i),1)=255;
    I(x(1,i),y(1,i),2)=0;
    I(x(1,i),y(1,i),3)=0;
end
figure(),imshow(I);
distance =round (sqrt((maxx0-secondMaxx0)*(maxx0-secondMaxx0)+(maxy0-secondMaxy0)*(maxy0-secondMaxy0)))
disX=(768-maxx0)/19.8+4.5;
disY=(maxy0)/19.8+5;
xPulse=disX*10/3*1000
yPulse=disY*10/3*1000



disX1=(768-secondMaxx0)/19.8+4.5;
disY1=(secondMaxy0)/19.8+5;
xPulse=disX1*10/3*1000
yPulse=disY1*10/3*1000
imwrite(I,'res5.jpg')
%imshow(L==); %转化为只有最大联通域的二值图像bw
%imshow(bw);
% [x,y]=size(g);
% bw = g>6;
% bw(1,1)=1;
% [a,b]=find(bw==0);
% %bw(:,750:y)=1;
% 
% %figure(),imshow(bw);
% bw(:,1000:y)=1;
% bw(750:end,:)=1;
% bw = 1-bw;
% imshow(bw);
% [xindex,yindex] = find(bw==1);
% xs = round(size(xindex,1)*0.1);

% XaxisXindex=xindex(xs:end-xs,:);
% XaxisYindex=yindex(xs:end-xs,:);
% tempbw=bw;
% tempbw(:)=1;
% size(XaxisXindex,1)
%  XaxisXindex
%  for i=1:size(XaxisXindex,1)
%      tempbw(XaxisXindex(i,:),XaxisYindex(i,:))=0;
%  end
% figure(),imshow(tempbw);


% 
% ys = round(size(yindex,1)*0.1);
% YaxisXindex=xindex(ys:end-ys,:);
% YaxisYindex=yindex(ys:end-ys,:);
% tempbw1=bw;
% tempbw1(:)=1;
% size(YaxisXindex,1)
%  
%  for i=1:size(YaxisXindex,1)
%      tempbw1(YaxisXindex(i,:),YaxisYindex(i,:))=0;
%  end
% figure(),imshow(tempbw1);
% thresh=[0.01,0.17]; 

%bw = bwmorph(bw,'remove');
% sigma=2;%定义高斯参数  
% f = edge(double(bw),'prewitt');  
% figure(),imshow(bw);  
% title('canny 边缘检测');

% bw = edge(imgray,'canny');
% figure(),imshow(bw);
% imhsv = rgb2hsv(I);
%figure(),imshow(imhsv(:,:,1)>0.6 & imhsv(:,:,2)>0.2 & imhsv(:,:,3)>0.3)
end