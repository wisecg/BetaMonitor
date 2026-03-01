NeSi3data = uproot.open("NeSi3Th0.root")["simData"]
NeSi3data = NeSi3data.arrays(filter_name = "*", library = 'pd')
NeSi3data = unpack('3', NeSi3data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi3 = NeSi3data.shape[0]
NeSi3data = NeSi3data[ (NeSi3data.detSQ_En >0)]
gc.collect()

NeSi4data = uproot.open("NeSi4Th0.root")["simData"]
NeSi4data = NeSi4data.arrays(filter_name = "*", library = 'pd')
NeSi4data = unpack('4', NeSi4data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi4 = NeSi4data.shape[0]
NeSi4data = NeSi4data[ (NeSi4data.detSQ_En >0)]
gc.collect()

NeSi5data = uproot.open("NeSi5Th0.root")["simData"]
NeSi5data = NeSi5data.arrays(filter_name = "*", library = 'pd')
NeSi5data = unpack('5', NeSi5data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi5 =     NeSi5data.shape[0]
NeSi5data = NeSi5data[ (NeSi5data.detSQ_En >0)]
gc.collect()

NeSi6data = uproot.open("NeSi6Th0.root")["simData"]
NeSi6data = NeSi6data.arrays(filter_name = "*", library = 'pd')
NeSi6data = unpack('6', NeSi6data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi6 =     NeSi6data.shape[0]
NeSi6data = NeSi6data[ (NeSi6data.detSQ_En >0)]
gc.collect()

NeSi7data = uproot.open("NeSi7Th0.root")["simData"]
NeSi7data = NeSi7data.arrays(filter_name = "*", library = 'pd')
NeSi7data = unpack('7', NeSi7data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi7 =     NeSi7data.shape[0]
NeSi7data = NeSi7data[ (NeSi7data.detSQ_En >0)]
gc.collect()

NeSi8data = uproot.open("NeSi8Th0.root")["simData"]
NeSi8data = NeSi8data.arrays(filter_name = "*", library = 'pd')
NeSi8data = unpack('8', NeSi8data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi8 =     NeSi8data.shape[0]
NeSi8data = NeSi8data[ (NeSi8data.detSQ_En >0)]
gc.collect()

NeSi9data = uproot.open("NeSi9Th0.root")["simData"]
NeSi9data = NeSi9data.arrays(filter_name = "*", library = 'pd')
NeSi9data = unpack('9', NeSi9data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi9 =     NeSi9data.shape[0]
NeSi9data = NeSi9data[ (NeSi9data.detSQ_En >0)]
gc.collect()

NeSi10data = uproot.open("NeSi10Th0.root")["simData"]
NeSi10data = NeSi10data.arrays(filter_name = "*", library = 'pd')
NeSi10data = unpack('10', NeSi9data, True, 'NeSi')#50,000,000 Shorter max step size->too far back
NeSi10 =     NeSi10data.shape[0]
NeSi10data = NeSi10data[ (NeSi10data.detSQ_En >0)]
gc.collect()

counts = NeSidata.shape[0] + NeSi2data.shape[0] + NeSi3data.shape[0] + NeSi4data.shape[0] + NeSi5data.shape[0] + NeSi6data.shape[0] + NeSi7data.shape[0] + NeSi8data.shape[0] + NeSi9data.shape[0] + NeSi10data.shape[0]
total = NeSi + NeSi2 + NeSi3 + NeSi4 + NeSi5 + NeSi6 + NeSi7 + NeSi8 + NeSi9 + NeSi10
prob = counts / total
sigma = np.sqrt(prob * (1-prob) / total)
print(prob * 100, ' +/- ', 3*sigma * 100)

plt.figure(figsize=(14,8))
res = 256
Q=3500
num_bins = np.linspace(0,Q,res)
#num_binsNe = np.arange(0,2220,12)
#num_binsNe = np.arange(0,3500,12)
plt.hist(resultNe.detSQ_En[(resultNe.detSQ_En >0)&(resultNe.detSQ_ID ==22)]*1000,#& (Srdata.pmtRT_En < 120)], 
         num_bins, alpha=0.5, density=False, label =      'Ne6-Loss PVT-gammas')
plt.hist(resultNe.detSQ_En[(resultNe.detSQ_En >0)&(resultNe.detSQ_ID ==11)]*1000,#& (Srdata.pmtRT_En < 120)], 
         num_bins, alpha=0.5, density=False, label =      'Ne6-Loss PVT-betas')
#plt.ylabel('Probability')
plt.ylabel('Counts')
plt.xlabel('Energy (keV)')
plt.axvline(x=750, c='r')
#plt.xlim(0, 2200)
#plt.ylim(0, 0.002)
#plt.yscale('log')
plt.legend()
plt.show()
#print('Ne = ', NeSidata[NeSidata.detSQ_En>0].shape[0]/NeSi*100, '%')
#print('Ne = ', NeSidata[NeSidata.detSQ_En>0].shape[0]/NeSi*100, '%')
print('Ne = ', resultNe[resultNe.detSQ_En>0].shape[0]/totalNe*100, '%')
print('Ne Betas= ', resultNe[(resultNe.detSQ_En>0)&(resultNe.detSQ_ID ==11)].shape[0]/totalNe*100, '%')