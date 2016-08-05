clc;
split='test'; data=boxesData_ICDAR2013('split',split);
nm=cell({'TextProposals-ICDAR-angle8'});
boxesEval('data',data,'names',nm,'thrs',.7,'show',3,'fName','ICDAR2013_3','col','r');
hold on;
nm=cell({'TextProposals-ICDAR-angle6'});
boxesEval('data',data,'names',nm,'thrs',.7,'show',3,'fName','ICDAR2013_3','col','k');

%nm=cell({'TextProposals-ICDAR-angle4'});
%boxesEval('data',data,'names',nm,'thrs',.7,'show',3,'fName','ICDAR2013_3','col','g');

nm=cell({'TextProposals-ICDAR-symmetry-labg-fast'});
boxesEval('data',data,'names',nm,'thrs',.7,'show',3,'fName','ICDAR2013_3','col','b');
s = {'nw','ne'};
[legh,objh,outh,outm] = legend('thres-20','thres-30','thres-90','Location',s{1});
saveas(gca,'angle.jpg');