xDoc = xmlread('../../SVT/test.xml');

allImageitems = xDoc.getElementsByTagName('image');
n=allImageitems.getLength; bbs=cell(n,1);
count_proposal = 0;
for k = 0:allImageitems.getLength-1
   thisImageitem = allImageitems.item(k);

   thisImage = thisImageitem.getElementsByTagName('imageName');
   thisElement = thisImage.item(0);
   fprintf('%s\n',char(thisElement.getFirstChild.getData));
   imname = char(thisElement.getFirstChild.getData);
   fprintf('%s\n',imname(5:9));
   bbs{k+1} = textread(['../../SVT/Result/merge_5map_no_angle/' imname(5:9) '.txt']);

   bbs{k+1} = sortrows(bbs{k+1},5);
   tmp = bbs{k+1}(:,1:4);              
   [tmp2,ia,ic] = unique(tmp,'rows');
   count_proposal = count_proposal + size(tmp2,1);
   bbs{k+1} = bbs{k+1}(ia,:);
   bbs{k+1} = sortrows(bbs{k+1},5);
end
disp(count_proposal/allImageitems.getLength)
save('boxes/TextProposals-SVT-merge-full-test','bbs')
eval_SVT;
