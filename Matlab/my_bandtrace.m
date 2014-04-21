function [BDim,BD]=my_bandtrace(BW)
%多次边界跟踪
BD = [];
BWT_tm = BW;
BDim=repmat(logical(uint8(0)),size(BW,1),size(BW,2));

%前一次跟踪后，仍有边界点则继续跟踪
while sum(sum(BWT_tm)) ~= 0
    [BD_tm,BWT_tm] = bandtrace(BWT_tm);
    if length(BD) < length(BD_tm)
        BD = BD_tm;
    end
end
%扫描结果描述
for i = 1 : length(BD)
    BDim(BD(i,1),BD(i,2)) = 1;
end