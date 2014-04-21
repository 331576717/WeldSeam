function [bw,I]=test11
I=imread('E:\���Ӷ�λ\Code\C++\Locate\Test\Color_120_0.jpg');
%I=imread('F:\������\���ղ��������\Color_120_' + picnum + '.jpg');
%I=imread(['F:\������\���ղ��������\Color_120_'  num2str(picnum)  '.jpg']);
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
%%% L�д洢��ͨ�����ǩ
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
    %%%�����ͨ����������������Ͻ��С���½죬�����
    if tempMax>averagepixelnum*1.2 || tempMax<averagepixelnum*0.5
        continue;
    end
    %���п��ܴ�С����ͨ����ı�ǩ
    possiblelabel=[possiblelabel,i];
end
possiblelabel
finallabel = possiblelabel(1,1);

if size(possiblelabel,2)~=1
    %����в�ֹһ����ͨ������������������ͨ������ƽ������ֵ����һ��ɸѡ
    posnum = size(possiblelabel,2);
    for j = 1:posnum
        %������ͨ�����лҶȾ�ֵ
        index = find(L == possiblelabel(1,j));
        indexnum = size(index,1);
        %��ͨ�����ƽ���Ҷ�ֵ
        averagepixelval = sum(imgray(index))/indexnum;
        %������һ��������ɸѡ��ʣ�µ���ͨ����Ӧ��Ŀ�������С�ӽ�
        %������Щ��С�����������ȡƽ���Ҷ�ֵ��С��һ��
        
        %%%%%%���ɣ��Ƿ���Ҫ��
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
alpha=0:pi/200:2*pi;%�Ƕ�[0,2*pi]
R=10;%�뾶
x=round(maxx0+R*cos(alpha));
y=round(maxy0+R*sin(alpha));
for i=1:size(x,2)
    I(x(1,i),y(1,i),1)=255;
    I(x(1,i),y(1,i),2)=0;
    I(x(1,i),y(1,i),3)=0;
end

[x y]=find(L==secfinallabel);
secondMaxx0=mean(x),secondMaxy0=mean(y)
alpha=0:pi/200:2*pi;%�Ƕ�[0,2*pi]
R=10;%�뾶
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