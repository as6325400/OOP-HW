ans=[]
sum = 0
with open("a.txt","r",encoding="utf-8") as f:
    a = f.read().split('\n')
    for i in a:
        p = i.split(' ')
        for j in p:
            if j == "load":
                sum += (int(p[p.index(j)+1]))
                ans.append(int(p[p.index(j)+1]))

ans_baseline=[]
sum_baseline=0
with open("baseline.txt","r",encoding="utf-8") as f:
    a = f.read().split('\n')
    for i in a:
        p = i.split(' ')
        for j in p:
            if j == "load":
                sum_baseline += (int(p[p.index(j)+1]))
                ans_baseline.append(int(p[p.index(j)+1]))
                
for i in range(len(ans)):
    temp=(ans[i]-ans_baseline[i])/ans_baseline[i]
    print("測資",i+1,".的maxLinkLoad: ",ans[i]," (","{:.2f}".format(temp*100),"% )",sep="")

print("總數: ",sum," 條")
print("比baseline少: ",sum-sum_baseline," 條")