//============================================================================
// Name        : Soru2.cpp
// Author      : Muhammed Salih Aydogan
// Date        : 25/08/2020
// Description : X = ±100 ve Y = ±100 birim olan bir koordinat sisteminde bu alan içerisinde,
//               başlangıç noktasını 0,0 kabul ederek 5 adet rastgele noktalar oluşturup, 
//               bu noktaların herbirine bir kez uğrayarak tekrar başlangıç noktasına gelen 
//               en kısa yörüngenin nokta koordinatlarını bulan bir program.
// Result      : (0,0)(29,-4)(54,60)(55,70)(-49,93)(-13,-98)(0,0)
//============================================================================

#include <bits/stdc++.h> 
#include <ctime>
#include <math.h>

using namespace std;

list<int> path; // Liste : En kısa yolu tutar 

// Rastgele sayılar üretir 
int rast(){
	int enFazla = 100, enAz = -100; // Üretilecek sayının aralığını tutar
	return rand() % (enFazla - enAz + 1 ) + enAz;
}

// İki nokta arasındaki uzaklığı döndürür
int distance(int x1, int y1, int x2, int y2){
	int d1=pow((x1-x2),2);
	int d2=pow((y1-y2),2);
	int d=sqrt(d1+d2);
	return d; 
}

// Liste elemanlarını yazdırmak için metod
template <typename T>
void yazdir(T& v){  
    typename T::iterator i = v.begin(); //İteratör ile başlangıç adresi alınır 
    while (i != v.end())
        cout << "(" << *i++ << "," << *i++ << ")";
    cout << endl;
}

// Kordinatları temsi eden iki vektör alır ve işler 
template <typename T>
void isle(vector<T>& v1, vector<T>& v2 ){           
    typename vector<T>::iterator i1 = v1.begin();   // Başlangıç adresini tutar
    typename vector<T>::iterator i2 = v2.begin();
    typename vector<T>::iterator i3, i4;

    int cost1 = 0;      // iki düğüm arasındaki uzaklığı tutar 
    int cost2 = 1000;   // iki düğüm arasındaki max uzaklık 
    static int value_x, value_y = 0; // Listeye eklenecek elemanı tutar 

    // varsayılan olarak (0,0) noktasından başlayarak birbirine yakın iki düğümü bulan döngü 
    while (i1 != v1.end() and i2 != v2.end()){
        cost1 = distance(value_x, value_y, *i1++, *i2++); 
        if (cost1 < cost2){
            cost2 = cost1;
            i3 = i1;
            i4 = i2;
        }
    }

    value_x = *(i3-1);      // Sonraki değerlendirme için yeni düğüm değerini tutar
    value_y = *(i4-1);
    path.push_back(value_x);   // Liste içine elemanı ekle
    path.push_back(value_y);
    v1.erase(i3-1);         // Vektör içindeki elemanı sil
    v2.erase(i4-1);
}

// Ana fonksiyon
int main(){
	srand(time(0));      // Zaman hareketi başlıyor 
    int x = 0;           // Rastgele değeri tutar
    vector<int> g1, g2;  // Vector tanımlaması 

    // Rastgele sayı üreterek (x,y) noktalarını temsil eden iki vektör oluşturur
    for (int i = 1; i <= 5; i++){
        x = rast();
        g1.push_back(x); 
        x = rast();
        g2.push_back(x); 
    }
    
    while (!g1.empty() and !g2.empty()) // Vektör içerisinde eleman var ise 
    {
        isle(g1, g2);
    }

    // Listenin başına ve sonuna (0,0) kordinatlarını ekleyelim
    path.push_front(0);
    path.push_front(0);
    path.push_back(0);
    path.push_back(0);

	cout << "En kısa yörünge : ";
    yazdir(path); // En kısa yolu içeren liste yazdırılır
    return 0;
}
