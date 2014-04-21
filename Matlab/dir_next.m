function [Nextdir]=dir_next(dir,step)
%step=0,顺时针旋转45度
%step=1,逆时针旋转90度
if step==0
    dir_N=[[-1,-1]',[-1,0]',[-1,1]',[0,1]',[1,1]',[1,0]',[1,-1]',[0,-1]'];
    for i = 1:8
        if dir' == dir_N(:,i)
            try Nextdir=dir_N(:,i+1);
            catch Nextdir=dir_N(:,1);
            end
            Nextdir=Nextdir';
        end
    end
elseif step==1
    dir_Y=[[-1,-1]',[1,-1]',[1,1]',[-1,1]';[-1,0]',[0,-1]',[1,0]',[0,1]'];
    if dir(1)*dir(2)==0
        j=3;
    else
        j=1;
    end
    for i = 1:4
        if dir' == dir_Y(j:j+1,i)
            try Nextdir=dir_Y(j:j+1,i+1);
            catch Nextdir=dir_Y(j:j+1,1);
            end
            Nextdir=Nextdir';
        end
    end
else
    error('The input arguments "step" must be a logical num');
end