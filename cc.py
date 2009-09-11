import os


cc = os.popen('find -name "*.cpp" |grep -v moc_ | xargs pmccabe | sort -nr')
results = {}
for l in cc:
    nro = l.split('\t')[0]
    if nro in results:
        results[nro] += 1
    else:
        results[nro] = 1


nros = results.keys()
nros.sort(cmp=lambda x,y: int(x) - int(y))

print 'cc\toccurrencies'
print '---------------------'
for i in nros:
    print i,'\t', results[i]



