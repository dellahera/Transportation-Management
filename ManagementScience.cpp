#include<bits/stdc++.h>
using namespace std;

struct Express{
	string nama;
	float max_capacity;
	int max_req;
	double cost;
	map<string, int> city;
};

struct Order{
	string asal;
	string tujuan;
	int qty;
	double berat, biaya;
};

struct Rute{
	vector<int> alur;
	int cost;
};

string S[9]= {"Jakarta", "Bandung","Yogyakarta","Semarang","Bogor","Depok","Surabaya","Malang"};
string P[8] = {"NCS", "JNE", "SICEPAT", "NINJA EXPRESS", "RPX", "BLIBLI EXPRESS SERVICE", "POS INDONESIA" };
Express data[15][15];
Express Provider[10];
Order Requ;
vector<Order> RuteProvider;
vector<Order> Req;
vector<Order> RuteKota;
int n, x[10], awal, akhir, kotabeda=0, kotasama=0, dll=0;
long long total =0;
bool compareMin(Express a, Express b) 
{ 
    return a.cost < b.cost ;
} 
bool compareMax(Order a, Order b) 
{ 
    return (a.qty*a.berat) > (b.qty * b.berat) ;
} 
int minDist(Rute dist[], bool flag[]) 
{ 
    int min = INT_MAX, min_index; 
    for (int i = 0; i < 15; i++) 
        if (flag[i] == false && dist[i].cost <= min) 
            min = dist[i].cost, min_index = i; 
    return min_index; 
} 

int printSolution(Rute dist[]) 
{ 
    printf("Vertex \t\t Distance from Source\n"); 
    for (int i = 0; i < 15; i++){
    	printf("%d \t\t %d\n", i, dist[i].cost); 
    	for(int j=0; j<dist[i].alur.size(); j++) cout<<dist[i].alur[j]<<"-";
	} 
} 


Rute Orderan(Order X){
	for(long long j=0; j<8; j++){
		if(S[j]==X.asal) awal = j+7;
		if(S[j]==X.tujuan) 	akhir= j+7;

	}
	
	Order temp;
	temp= X;
	Rute dist[15];
	bool flag[15];
	//Rute dengan kota yang sama
	if(awal == akhir){
		dist[awal].alur.push_back(awal);
		for(int i=0; i<7; i++){		
			if( Provider[i].city[temp.asal]==1 && temp.qty <= Provider[i].max_capacity && Provider[i].max_req>0){
				dist[awal].alur.push_back(i);
				dist[awal].cost = data[i][awal].cost* 2;
				break;
			}
		}
	}
	else { //Rute yang melewati provider berbeda
		for (int i = 0; i < 15; i++) 
        dist[i].cost = INT_MAX, flag[i] = false;
    
    	dist[awal].cost = 0;
    
   	 	for (int i = 0; i < 14; i++) {  
        	int u = minDist(dist, flag); 
        	flag[u] = true; 
        	for (int j = 0; j < 15; j++) {
    		//	printf("%d \t\t%d \t\t %d\t %d\n",i, j, dist[j].cost, u); 
            	if (!flag[j] && data[u][j].cost>0 && dist[u].cost != INT_MAX 
                	&& dist[u].cost + data[u][j].cost < dist[j].cost && temp.qty <= data[u][j].max_capacity && data[u][j].max_req >0) {
                		dist[j].cost = dist[u].cost + data[u][j].cost;
						dist[j].alur.push_back(u);
  						dist[j].alur.insert(dist[j].alur.begin(), dist[u].alur.begin(), dist[u].alur.end());
			//	cout<<u<<"=="<<j<<" "<<i<<" "<<dist[j].cost;
		 	//	cout<<"haha"<<endl;
				}  
			}
   	 	} 
   	 	//printSolution(dist); 
	}
	
	return dist[akhir];
}

int main(){

 		//Pendataan seluruh provider dan rute
	for(long long i=0; i<7; i++){
		cout<<"Masukkan data provider :";
		cin>>Provider[i].nama>>Provider[i].max_capacity>>Provider[i].max_req>>Provider[i].cost;			
	}

	sort(Provider, Provider+7, compareMin); 
	for(long long i=0; i<7; i++){
		cout<<Provider[i].nama<<" :";
		cout<<"Kode kota : 1.Jakarta 2.Bandung 3.Yogyakarta 4.Semarang 5.Bogor 6.Depok 7.Surabaya 8.Malang"<<endl;
		cout<<"Masukkan banyak kota = ";
		cin>>n;
		for(long long j=1; j<=n; j++){
			cin>>x[j];
			for(long long k=j; k>=1; k--){
			data[i][6+x[k]]=Provider[i];
			data[6+x[j]][i]=Provider[i];
			data[i][6+x[k]].cost=Provider[i].cost/2;
			data[6+x[j]][i].cost=Provider[i].cost/2;
			Provider[i].city[S[x[j]-1]]=1;
			}	
		}
			
	}
	for(long long i=0; i<7; i++){
		cout<<"Nama Provider : "<<Provider[i].nama<<endl;
		cout<<setw(20)<<" Kapasitas Max "<<Provider[i].max_capacity<<setw(5)<<", Request Max "<<Provider[i].max_req<<setw(5)<<", Harga : "<<Provider[i].cost<<endl;
	}
	
	//Input daftar pengiriman barang
	cout<<"Masukkan banyak orderan : ";
	cin>>n;
	cout<<"Masukkan data orderan : ";
	for(long long i=0; i<n; i++){
		cin>>Requ.asal>>Requ.tujuan>>Requ.qty>>Requ.berat;
		bool cek=0;
		//Filter orderan per rute tiap provider
		for(long long i=0; i<7; i++){
			if(Provider[i].city[Requ.asal]==1 && Provider[i].city[Requ.tujuan]==1 && Requ.asal!=Requ.tujuan){
				RuteProvider.push_back(Requ);
				kotabeda++;
				cek=1;
				break;
			}
			else if(Requ.asal==Requ.tujuan) {
				Req.push_back(Requ); 
				kotasama++;
				cek=1;
				break;
			}	
		}
		if(!cek) {
			Req.push_back(Requ);  
			dll++;	
		}
	}

	cout<<"\nOrderan beda kota 1 Provider "<<setw(5)<<kotabeda<<"\nOrderan Kota yang sama "<<setw(5)<<kotasama<<"\nOrderan Beda Provider "<<setw(5)<<Req.size()-kotasama<<endl;	
	//Proses orderan
	int jum=1,  countt= 0;
	// Rute beda kota dengan 1 provider
	sort(RuteProvider.begin(), RuteProvider.end(), compareMax);
	for(int i=0; i<RuteProvider.size(); i++){
		Rute dataReq ;
		cout<<jum<<" "<<RuteProvider[i].asal<<" "<<RuteProvider[i].tujuan<<" "<<RuteProvider[i].qty<<" "<<RuteProvider[i].berat<<endl;
		dataReq = Orderan(RuteProvider[i] );
		cout<<setw(10)<<jum<<" Alur : ";
		for(int j=0; j<dataReq.alur.size(); j++){
			int temp= dataReq.alur[j];
			if(temp<7) {
				for(int k=7; k<15; k++){
					data[temp][k].max_capacity -= RuteProvider[i].qty;
					data[temp][k].max_req--;
				}
				RuteProvider[i].biaya += (RuteProvider[i].berat * data[temp][j].cost);	
				cout<<P[temp]<<" ";
			}
            else cout<<S[temp-7]<<" "; 	
		}  
		long long bil= dataReq.cost * RuteProvider[i].berat;
		total+= bil;
			if(dataReq.cost== INT_MAX) countt++;
		cout<<RuteProvider[i].tujuan<< " Total biaya = "<<dataReq.cost<<" x "<<RuteProvider[i].berat <<" = "<<bil<<endl<<endl; 
		jum++;
    } 
    
    // Rute lain
    sort(Req.begin(), Req.end(), compareMax);
	for(int i=0; i<Req.size(); i++){
		cout<<jum<<" "<<Req[i].asal<<" "<<Req[i].tujuan<<" "<<Req[i].qty<<" "<<Req[i].berat<<endl;
		Rute dataReq ;
		dataReq = Orderan(Req[i] );
		cout<<setw(10)<<" Alur : ";
		for(int j=0; j<dataReq.alur.size(); j++){
			int temp= dataReq.alur[j];
			if(temp<7) {
				for(int k=7; k<15; k++){
					if(data[temp][k].max_capacity!= 0) data[temp][k].max_capacity -= Req[i].qty;
					if(data[temp][k].max_req!= 0) data[temp][k].max_req--;
				}
				Req[i].biaya += (Req[i].berat  * data[temp][j].cost);	
				cout<<P[temp]<<" ";
			}
            else cout<<S[temp-7]<<" "; 	
		}  
		long long bil= dataReq.cost * Req[i].berat;
		total+= bil;
		if(dataReq.cost== INT_MAX) countt++;
		cout<<Req[i].tujuan<< " Total biaya = "<<dataReq.cost<<" x "<<Req[i].berat <<" = "<<bil<<endl<<endl; 
		jum++;
    } 
    int jlh=0;
    for(long long i=0; i<7; i++){
    	int temp=0;
		for(int j=7; j<15; j++){
			temp= max(temp, data[i][j].max_req);
			//cout<<i<<" "<<j<<" "<<data[i][j].max_capacity<<" "<<data[i][j].max_req<<endl;
		}
		jlh+=temp;
	}
//	cout<<endl<<jlh<<" "<<countt;
	cout<<endl<<"Total pengeluaran = "<<total<<endl;
 }

