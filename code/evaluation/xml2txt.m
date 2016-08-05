xDoc = xmlread('../../SVT/svt1/test.xml');

allImageitems = xDoc.getElementsByTagName('image');
% n=allImageitems.getLength; bbs=cell(n,1);
% for k = 0:allImageitems.getLength-1
%     k
%    thisImageitem = allImageitems.item(k);
% 
%    thisImage = thisImageitem.getElementsByTagName('imageName');
%    thisElement = thisImage.item(0);
%   
%    fprintf('%s\n',char(thisElement.getFirstChild.getData));
%    imname = char(thisElement.getFirstChild.getData);
%    fprintf('%s\n',imname(5:9));
%    path = ['../../SVT/svt1/' imname];
%    savepath = ['../../SVT/Images/train/' imname(5:13)];
%    copyfile(path,savepath); 
% end
%get annotation
for k = 0:allImageitems.getLength-1
    k;
   thisImageitem = allImageitems.item(k);
   thisImage = thisImageitem.getElementsByTagName('imageName');
   thisElement = thisImage.item(0);
   
   imname = char(thisElement.getFirstChild.getData);
   fprintf('%s\n',imname(5:9));
   savepath = ['../../SVT/Annotation/test/gt_' imname(5:9) '.txt'];
   fid=fopen(savepath,'w');
   
   thisImage = thisImageitem.getElementsByTagName('taggedRectangles');
   temp = thisImage.item(0);
   Num =temp.getElementsByTagName('taggedRectangle');
   for i = 0:Num.getLength -1
        thisrectangle = Num.item(i);
        x = char(thisrectangle.getAttribute('x'));
        %x = int16(x);
        y = char(thisrectangle.getAttribute('y'));
        h = char(thisrectangle.getAttribute('height'));
        w = char(thisrectangle.getAttribute('width'));
        word = thisrectangle.getElementsByTagName('tag');
        word2 = word.item(0).getFirstChild.getData;
        data = [x y w h];
        fprintf(fid,'%s\t',x);
        fprintf(fid,'%s\t',y);
        fprintf(fid,'%s\t',w); 
        fprintf(fid,'%s\t',h);
        fprintf(fid,'\n');
   end 
   
   sta = fclose(fid);
%   
%    path = ['../../SVT/svt1/' imname];
%    savepath = ['../../SVT/Images/train/' imname(5:13)];
%    copyfile(path,savepath); 
end