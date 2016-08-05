if 0
xDoc = xmlread('/path/to/datasets/ICDAR2013/test_locations.xml');
allImageitems = xDoc.getElementsByTagName('image');
n=allImageitems.getLength; bbs=cell(n,1);

for k = 0:allImageitems.getLength-1
   thisImageitem = allImageitems.item(k);

   thisImage = thisImageitem.getElementsByTagName('imageName');
   thisElement = thisImage.item(0);
   fprintf('%s\n',char(thisElement.getFirstChild.getData));
   imname = char(thisElement.getFirstChild.getData);
   fprintf('%s\n',imname(strfind(imname,'_')+1:strfind(imname,'.')-1));
   bbs{k+1} = textread(['../data/' imname(strfind(imname,'_')+1:strfind(imname,'.')-1)]);

   bbs{k+1} = sortrows(bbs{k+1},5);
   tmp = bbs{k+1}(:,1:4);              
   [tmp2,ia,ic] = unique(tmp,'rows');
   bbs{k+1} = bbs{k+1}(ia,:);
   bbs{k+1} = sortrows(bbs{k+1},5);
end

save('boxes/TextProposals-ICDAR-FULL-test','bbs')
end

if 0 %% text proposals %%
clc;clear all;
data_dir = '../dataset/icdar_2013/test/image/';
load('boxes/GroundTruth-ICDAR2013-test.mat');%data
inds = data.ids;
bbs = cell(length(inds),1);
for i=1:length(inds)
   name = char(inds(i));
   img_path = [data_dir,name];
   proposal_path = [img_path,'.txt'];
   bbs{i} = textread(proposal_path);%,'%d %d %d %d %f')
   bbs{i} = sortrows(bbs{i},5);
   tmp = bbs{i}(:,1:4);
   [tmp2,ia,ic]=unique(tmp,'rows');
   bbs{i} = bbs{i}(ia,:);
   bbs{i} = sortrows(bbs{i},5);
   disp(i)
end
save('boxes/TextProposals-ICDAR-FULL-test','bbs')
end
%imgData = dir([data_dir,'*.jpg'])

if 1%% edge box %%
    
clc;clear all
data_dir = '../../ICDAR2013/Result/angel_6/';
load('boxes/GroundTruth-ICDAR2013-test.mat');%data
inds = data.ids;
bbs = cell(length(inds),1);
count_proposal = 0;
for i=1:length(inds)
   name = char(inds(i));
   img_path = [data_dir,name];
   %proposal_path = [img_path(1:end-4)];
   proposal_path = [img_path(1:end-4),'.txt'];
   bbs{i} = textread(proposal_path);%,'%d %d %d %d %f')%the scored is right?
   bbs_t = bbs{i};
   if(isempty(bbs_t))
       bbs{i} = zeros(0,5);
       continue;
   end
   bbs_t = [bbs_t(:,1),bbs_t(:,2),bbs_t(:,3),bbs_t(:,4),1*bbs_t(:,5)];%multiplye -1 is need
   bbs_t = sortrows(bbs_t,5);
   tmp = bbs_t(:,1:4);
   [tmp2,ia,ic]=unique(tmp,'rows');
   bbs_t = bbs_t(ia,:);
   count_proposal = count_proposal + size(bbs_t,1);
   %[y] = sortrows(bbs_t,5);
   bbs{i} = sortrows(bbs_t,5);
   disp(i)
end
disp(count_proposal/length(inds))
save('boxes/TextProposals-ICDAR-angle12-test','bbs')
end


