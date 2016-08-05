cnts=[1 2 5 10 20 50 100 200 500 1000 2000 5000 10000 15000]; col=cell(100,1);
for i=1:100, col{i}=max(.3,mod([.3 .47 .16]*(i+1),1)); end
dfs={ 'data','REQ', 'names','REQ', 'resDir','boxes/', 'thrs',.7, ...
  'cnts',cnts, 'maxn',inf, 'show',1, 'fName','', 'col',col };
o=getPrmDflt(varargin,dfs,1);
M = 14;T = 3;K = 1;
recall=zeros(M,T,K); 
[ms,ts,ks]=ndgrid(1:M,1:T,1:K);
rdir{3} = ; 
for i=1:M*T*K, 
    m=ms(i); t=ts(i); k=ks(i);
    rnm=[rdir 'N' int2str2(n,5) '-W' int2str2(o.cnts(m),5) ...
    '-T' int2str2(round(o.thrs(t)*100),2) '.txt']; %#ok<*PFBNS>
    if(exist(rnm,'file')), 
        recall(i)=load(rnm,'-ascii'); 
        continue;
    end
end