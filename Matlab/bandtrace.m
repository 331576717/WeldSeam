function [BD,BWT]=bandtrace(BW)
%���α߽�׷��
%[BW,Lt]=im2bw_t(I,T);
BD = [];%�߽�����꼯��
BWT = BW;%������ɨ��ı߽����ֵͼ��


ST = fnd_stP(BWT);
dir = [-1,-1];
%NextPoint(r,c)=[r,c]+dir
%��ʼΪ���Ϸ�
Point = ST;
BD = [BD;ST];
NextP = Point + dir;
while (NextP(1)*NextP(2)==0) | (NextP(1)>=size(BWT,1)) | (NextP(2)>=size(BWT,2))
    dir = dir_next(dir,0);
    NextP = Point + dir;
end
BWT(Point(1),Point(2)) = 0;%������ʼ��
%BWT(NextP(1),NextP(2))
%while
totle = sum(sum(BWT));
i = 1;j = 0;
while (i<=totle) & ((NextP(1)~=ST(1)) | (NextP(2)~=ST(2)))
    if BWT(NextP(1),NextP(2)) == 0
        if j >= 8
            NextP = Point;
            break;
        end
        j = j + 1;
        dir = dir_next(dir,0);
        NextP = Point + dir;
    else
        j = 0;
        %���Ϊ�߽��
        BD = [BD;NextP];
        BWT(NextP(1),NextP(2)) = 0;%����
        Point = NextP;
        dir = dir_next(dir,1);
        NextP = Point + dir;
        i = i + 1;
    end
    while (NextP(1)*NextP(2)==0) | (NextP(1)>=size(BWT,1)) | (NextP(2)>=size(BWT,2))
        dir = dir_next(dir,0);
        NextP = Point + dir;
    end
end