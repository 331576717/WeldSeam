function [bw,I]=test11
I=imread('E:\焊接定位\Code\C++\Locate\Test\Color_120_0.jpg');
%I=imread('F:\机器人\光照不均匀情况\Color_120_' + picnum + '.jpg');
%I=imread(['F:\机器人\光照不均匀情况\Color_120_'  num2str(picnum)  '.jpg']);
%imgray = I(:,:,1)*0.7 + I(:,:,2)*0.2 + I(:,:,3)*0.1;
imgray=rgb2gray(I);
%figure(),imshow(I);
w=fspecial('gaussian',[10 10],0.5);
imgray = imfilter(imgray,w,'replicate');
%figure(),imshow(imgray);
imgray = medfilt2(imgray,[6 6]);
%figure(),imshow(imgray);
se=strel('rectangle',[20 10]);
g=imsubtract(imadd(imgray,imtophat(imgray,se)),imbothat(imgray,se));
%figure(),imshow(g);
%imhist(g)

%figure(),imshow(g>30);
%%%%---figure,imshow(g);
bw=logical(g<50);
bw(:,1000:end)=0;
bw(750:end,:)=0;
%%%---figure,imshow(bw);
%%% L中存储联通区域标签
[L, num]=bwlabeln(bw,4);

maxIndex=[0,0];
secondMaxIndex=[0,0];
%%%
averagepixelnum = 2000;%%
%averagepixelval = [];
normalaveragepixval = 40;%%
minaveragepixval = 90;%%
secaveragepixval = 90;%%
finallabel = 0;
secfinallabel = 0;
possiblelabel=[];
%%%
for i=1:num
    tempMax=size(find(L==i),1);
    %%%如果联通区域的像素数大于上届或小于下届，则忽略
    if tempMax>averagepixelnum*1.2 || tempMax<averagepixelnum*0.5
        continue;
    end
    %所有可能大小的联通区域的标签
    possiblelabel=[possiblelabel,i];
end
possiblelabel
finallabel = possiblelabel(1,1);

if size(possiblelabel,2)~=1
    %如果有不止一个联通区域像素总数合理则通过区域平均像素值来进一步筛选
    posnum = size(possiblelabel,2);
    for j = 1:posnum
        %计算联通区域中灰度均值
        index = find(L == possiblelabel(1,j));
        indexnum = size(index,1);
        %连通区域的平均灰度值
        averagepixelval = sum(imgray(index))/indexnum;
        %经过第一步像素数筛选后，剩下的联通区域应与目标区域大小接近
        %则在这些大小相近的区域中取平均灰度值最小的一个
        
        %%%%%%存疑？是否需要？
        if(averagepixelval > normalaveragepixval*1.2 || averagepixelval < normalaveragepixval*0.6)
            continue;
        end;
  
        %finallabel =
        if (averagepixelval <= secaveragepixval)
            if averagepixelval <= minaveragepixval
                secfinallabel = finallabel;
                secaveragepixval = minaveragepixval;
                finallabel = possiblelabel(1,j);
                minaveragepixval = averagepixelval;
            else
                secfinallabel = possiblelabel(1,j);
                secaveragepixval = averagepixelval;
            end
        end;                    
    end    
end


finallabel,secfinallabel
a = (L == finallabel);
b = (L == secfinallabel);
d = a + b;
bw(:) = 1;
bw(d == 1) = 0;
%%%%---figure(),imshow(bw);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%figure(),imshow(L==maxIndex(1,2) | L==secondMaxIndex(1,2));

[x y]=find(L==finallabel);
maxx0=mean(x),maxy0=mean(y);
alpha=0:pi/200:2*pi;%角度[0,2*pi]
R=10;%半径
x=round(maxx0+R*cos(alpha));
y=round(maxy0+R*sin(alpha));
for i=1:size(x,2)
    I(x(1,i),y(1,i),1)=255;
    I(x(1,i),y(1,i),2)=0;
    I(x(1,i),y(1,i),3)=0;
end

[x y]=find(L==secfinallabel);
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
maxx0;
maxy0;
distance =round (sqrt((maxx0-secondMaxx0)*(maxx0-secondMaxx0)+(maxy0-secondMaxy0)*(maxy0-secondMaxy0)))
disX=(768-maxx0)/19.8+4.5
disY=(maxy0)/19.8+5
xPulse=disX*10/3*1000
yPulse=disY*10/3*1000



disX1=(768-secondMaxx0)/19.8+4.5
disY1=(secondMaxy0)/19.8+5
xPulse=disX1*10/3*1000
yPulse=disY1*10/3*1000
end