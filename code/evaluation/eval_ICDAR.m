% run and evaluate on entire dataset (see boxesData.m and boxesEval.m)
%addpath('../releaseV3')

split='test'; data=boxesData_ICDAR2013('split',split);
%nm=cell({'TextProposals-ICDAR-symmetry-labg-fast'});
%nm=cell({'TextProposals-ICDAR-FULL-symmetry-scale-4-4map-l2-boost50'});
nm=cell({'TextProposals-ICDAR-boost'});

boxesEval('data',data,'names',nm,'thrs',.5,'show',2,'fName','ICDAR2013_2');
boxesEval('data',data,'names',nm,'thrs',.7,'show',3,'fName','ICDAR2013_3');
%hold on;
boxesEval('data',data,'names',nm,'thrs',.9,'show',4,'fName','ICDAR2013_4');
boxesEval('data',data,'names',nm,'thrs',.5:.05:1,'cnts',1000,'show',5,'fName','ICDAR2013_5');
boxesEval('data',data,'names',nm,'thrs',.5:.05:1,'cnts',5000,'show',6,'fName','ICDAR2013_6');
boxesEval('data',data,'names',nm,'thrs',.5:.05:1,'cnts',10000,'show',7,'fName','ICDAR2013_7');
hold off;