#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
using namespace std;
long long comp = 0, mov=0,rw=0;
struct toy
{
    string name;
    int price;
    int from_age;
    int to_age;
   
};
typedef struct toy;
ostream& operator<<(std::ostream& out, const toy& t)
{
    return out << t.name << " " << t.price << " " << t.from_age << " " << t.to_age << endl;
}
istream& operator>>(std::istream& in, toy& t)
{
    return in >> t.name >> t.price >> t.from_age >> t.to_age;
}
void quick_inner(toy* a, int n, int l, int r) {
    comp++;
    if (l < r) { // проверяем меньше ли левая граница правой, иначе
                                            // завершаем ветвь рекурсии
        toy mid = a[(l + r) / 2]; // выбор опорного элемента
        int ll = l;
        int rr = r;
        mov += 3;
        comp++;
        while (ll <= rr) // пока текуший проверяемый левый индекс не 
                            // больше правого индекса
        {
            comp++;
            // поиск пары элементов которых нужно поменять местами
            comp++; while (a[ll].name.compare(mid.name) < 0) {
                ll++; comp++; mov++;
            }
            comp++;  while (a[rr].name.compare(mid.name) > 0) {
                rr--; comp++; mov++;
            }
            comp++;
            if (ll <= rr) { // меняем эти элементы месами
                toy t = a[ll];
                a[ll] = a[rr];
                a[rr] = t;
                ll++;
                rr--;
                mov += 5;
            }
        }
        comp += 2;
        if (ll < r) quick_inner(a, n, ll, r); // рекурсивный вызов сортировки
                                               //правой части
        if (rr > l) quick_inner(a, n, l, rr); // рекурсивный вызов сортировки
                                                //левой части
    }
}
//i fix it
void merge(fstream &f1, fstream &f2, fstream &f3, string a, string b, string c, toy &buf, toy &buf2, int &in_b,int &in_c,int &r_block_size,bool &need_to_read_b,bool &need_to_read_c) {
    f1.open(a, ios::out); f2.open(b, ios::in); f3.open(c, ios::in);
    int kol_it = max(in_b, in_c);
    kol_it = min(in_b, in_c) / r_block_size + 1; //кол-во блоков, которых необходимо слить
    mov += 1; comp++;
    for (int i = 0; i < kol_it; i++) {
        int cur_por_left_f2 = 0, cur_por_left_f3 = 0; //сколько элементов в блоке осталось
        //если размер блока меньше остатка в файле то остаток = размер блока
        //иначе это оставшееся кол-во в файле
        mov += 4; comp += 2;
        if (r_block_size <= in_b) cur_por_left_f2 = r_block_size;
        else cur_por_left_f2 = in_b;
        if (r_block_size <= in_c) cur_por_left_f3 = r_block_size;
        else cur_por_left_f3 = in_c;
        comp++;
        while (cur_por_left_f2 > 0 && cur_por_left_f3 > 0) {//пока оба блока не пусты
            //читаем элемент, если блок не пустой и из этого файла нужно прочитать следующий 
            //элемент
            comp += 3;
            if (need_to_read_b && in_b > 0) f2 >> buf;
            if (need_to_read_c && in_c > 0) f3 >> buf2;
            comp++;
            if (buf.name.compare(buf2.name) < 0) {//сравниваем и меньший записываем в f1
                f1 << buf; rw++; mov += 2;
                cur_por_left_f2--; in_b--;
                comp += 2; mov += 2;
                if (cur_por_left_f2) {//в каком файле нужно прочитать следующее число
                    need_to_read_b = true; need_to_read_c = false;
                }
                else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
                    need_to_read_b = true;
                    need_to_read_c = true;
                }
            }
            else {
                f1 << buf2; rw++; mov += 2;
                cur_por_left_f3--; in_c--;
                comp += 2; mov += 2;
                if (cur_por_left_f3) {//в каком файле нужно прочитать следующее число
                    need_to_read_b = false; need_to_read_c = true;
                }
                else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
                    need_to_read_b = true;
                    need_to_read_c = true;
                }
            }
        }
        comp++;
        if (in_b == 0 && in_c > 0) {//если файл закончился, то остаток другого переливаем
            comp++;
            while (in_c > 0) {
                comp++; mov++; rw += 2;
                in_c--;
                if (r_block_size == 1) {
                    f3 >> buf2;
                    f1 << buf2;
                }
                else {
                    f1 << buf2;
                    f3 >> buf2;
                }
            }
        }
        comp++;
        if (in_c == 0 && in_b > 0) {//если файл закончился, то остаток другого переливаем
            comp++;
            while (in_b > 0) {
                comp++; mov++; rw += 2;
                in_b--;
                if (r_block_size == 1) {
                    f2 >> buf;
                    f1 << buf;
                }
                else {
                    f1 << buf;
                    f2 >> buf;
                }
            }
        }
        //после while один из блоков пуст, поэтому переливаем в f1 остаток другого блока
        comp += 2;
        if (in_b > 0 && cur_por_left_f2 > 0) {
            comp++;
            while (cur_por_left_f2 > 0) {
                comp++; rw++; mov += 2;
                f1 << buf; in_b--;
                cur_por_left_f2--;
                comp++;
                if (in_b > 0 && cur_por_left_f2) { f2 >> buf; rw++; }
            }
            mov += 2;
            need_to_read_b = true; need_to_read_c = true;
        }
        else if (in_c > 0 && cur_por_left_f3 > 0) {
            comp++;
            while (cur_por_left_f3 > 0) {
                comp++; rw++; mov += 2;
                f1 << buf2; in_c--;
                cur_por_left_f3--;
                comp++;
                if (in_c > 0 && cur_por_left_f3) {
                    f3 >> buf2; rw++;
                }
            }
            mov += 2;
            need_to_read_b = true; need_to_read_c = true;
        }
    }
    comp++;
    need_to_read_b = true; need_to_read_c = true;
    r_block_size *= 2;//увеличиваем размер блока в 2 раза
    f1.close(); f2.close(); f3.close();
}
void mergesort(string a, string b, string c, int n) {//название файлов a,b,c, и кол-во элементов - параметры
    fstream f1, f2, f3;
    toy buf,buf2;
    bool bcfile = true;
    bool need_to_read_b = true, need_to_read_c = true;
    int r_block_size = 1, in_b=0, in_c=0;
    mov += 6; comp++;
    while (r_block_size < n) {//пока размер сливаемого блока меньше кол-ва элементов
        int ii = 0; comp++; mov += 3;
        in_b = 0; in_c = 0;//кол-во элементов в соответствующих файлах
        f1.open(a, ios::in); f2.open(b, ios::out); f3.open(c, ios::out);
        comp++;
        while (ii < n) {  //перезаписываем блоки из а поочередно в b и с
            comp += 2;
            if (bcfile) { //запись в b 
                comp++;
                for (int i = 0; i < r_block_size; i++) {
                    comp++;
                    f1 >> buf;
                    f2 << buf; in_b++;
                    ii++;
                    comp++;
                    if (ii >= n) break;
                    rw += 2;
                }
                bcfile = false;
            }
            else {//запись в с
                comp++;
                for (int i = 0; i < r_block_size; i++) {
                    comp += 2;
                    f1 >> buf;
                    f3 << buf; in_c++;
                    ii++;
                    if (ii >= n) break; 
                    rw += 2;
                }
                bcfile = true;
            } 
        }
        f1.close(); f2.close(); f3.close();
        //этап слияния
        f1.open(a, ios::out); f2.open(b, ios::in); f3.open(c, ios::in);
        int kol_it = max(in_b, in_c);
        kol_it = min(in_b, in_c)/r_block_size+1; //кол-во блоков, которых необходимо слить
        mov += 1; comp++;
        for (int i = 0; i < kol_it; i++) {
            int cur_por_left_f2 = 0, cur_por_left_f3 = 0; //сколько элементов в блоке осталось
            //если размер блока меньше остатка в файле то остаток = размер блока
            //иначе это оставшееся кол-во в файле
            mov += 4; comp += 2;
            if (r_block_size <= in_b) cur_por_left_f2 = r_block_size;
            else cur_por_left_f2 = in_b;
            if (r_block_size <= in_c) cur_por_left_f3 = r_block_size;
            else cur_por_left_f3 = in_c;
            comp++;
            while (cur_por_left_f2 > 0 && cur_por_left_f3 > 0) {//пока оба блока не пусты
                //читаем элемент, если блок не пустой и из этого файла нужно прочитать следующий 
                //элемент
                comp += 3;
                if (need_to_read_b && in_b > 0) f2 >> buf;
                if (need_to_read_c && in_c > 0) f3 >> buf2;
                comp++;
                if(buf.name.compare(buf2.name)<0){//сравниваем и меньший записываем в f1
                    f1 << buf; rw++; mov += 2;
                    cur_por_left_f2--; in_b--;
                    comp += 2; mov += 2;
                    if (cur_por_left_f2) {//в каком файле нужно прочитать следующее число
                        need_to_read_b = true; need_to_read_c = false;
                    }
                    else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
                        need_to_read_b = true;
                        need_to_read_c = true;
                    }
                }
                else {
                    f1 << buf2; rw++; mov += 2;
                    cur_por_left_f3--; in_c--;
                    comp += 2; mov += 2;
                    if (cur_por_left_f3) {//в каком файле нужно прочитать следующее число
                        need_to_read_b = false; need_to_read_c = true;
                    }
                    else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
                        need_to_read_b = true;
                        need_to_read_c = true;
                    }
                }
            }
            comp++;
            if (in_b == 0 && in_c > 0) {//если файл закончился, то остаток другого переливаем
                comp++;
                while (in_c > 0){
                    comp++; mov++; rw += 2;
                    in_c--;
                    if (r_block_size == 1) {
                        f3 >> buf2;
                        f1 << buf2;
                    }
                    else {
                        f1 << buf2;
                        f3 >> buf2;
                    }
                }
            }
            comp++;
            if (in_c == 0 && in_b > 0) {//если файл закончился, то остаток другого переливаем
                comp++;
                while (in_b > 0) {
                    comp++; mov++; rw += 2;
                    in_b--; 
                    if (r_block_size == 1) {
                        f2 >> buf;
                        f1 << buf;
                    }
                    else {
                        f1 << buf;
                        f2 >> buf;
                    }
                }
            }
            //после while один из блоков пуст, поэтому переливаем в f1 остаток другого блока
            comp+=2;
            if (in_b > 0 && cur_por_left_f2 > 0) {
                comp ++;
                while (cur_por_left_f2 > 0) {
                    comp++; rw++; mov += 2;
                    f1 << buf; in_b--;
                    cur_por_left_f2--;
                    comp++;
                    if (in_b > 0 && cur_por_left_f2) { f2 >> buf; rw++; }
                }
                mov += 2;
                need_to_read_b = true; need_to_read_c = true;
            } 
            else if (in_c > 0 && cur_por_left_f3 > 0) {
                comp++;
                while (cur_por_left_f3 > 0) {
                    comp++; rw++; mov += 2;
                    f1 << buf2; in_c--;
                    cur_por_left_f3--;
                    comp++;
                    if (in_c > 0 && cur_por_left_f3) {
                        f3 >> buf2; rw++;
                    }
                }
                mov += 2;
                need_to_read_b = true; need_to_read_c = true;
            }
        }
        comp++;
        need_to_read_b = true; need_to_read_c = true;
        r_block_size *= 2;//увеличиваем размер блока в 2 раза
        f1.close(); f2.close(); f3.close();
    }
}  
void hybrid_mergesort(string a, string b, string c, int n,int bars) {//название файлов a,b,c, и кол-во элементов - параметры
    fstream f1, f2, f3;
    toy buf, buf2;
    bool bcfile = true;
    bool need_to_read_b = true, need_to_read_c = true;
    int r_block_size = 1, in_b = 0, in_c = 0;
    mov += 6; comp++;
    f1.open(a, ios::in); f2.open(b, ios::out); f3.open(c, ios::out);
   
    int nn = n,arra_s;
    toy *arra; 
    while (nn > 0) {
        if (nn >= bars) {
            arra = new toy[bars]; arra_s = bars;
        }
        else {
            arra = new toy[nn]; arra_s = nn;
        }
        for (int i = 0; i < arra_s; i++) {
            toy aaaa;
            f1 >> aaaa;
            arra[i] = aaaa; nn--;
        }
        quick_inner(arra, arra_s, 0, arra_s - 1);
        comp += 2;

        if (bcfile) { //запись в b 
            comp++;
            for (int i = 0; i < arra_s; i++) {
                comp++;
                f2 << arra[i]; in_b++;
                comp++;
                rw += 2;
            }
            bcfile = false;
        }
        else {//запись в с
            comp++;
            for (int i = 0; i < arra_s; i++) {
                comp += 2;
                f3 << arra[i]; in_c++;
                rw += 2;
            }
            bcfile = true;
        }
    }
    f1.close(); f2.close(); f3.close();
    r_block_size = bars;

    merge(f1, f2, f3, a, b, c, buf, buf2, in_b, in_c, r_block_size, need_to_read_b, need_to_read_c);

    while (r_block_size < n) {//пока размер сливаемого блока меньше кол-ва элементов
        int ii = 0; comp++; mov += 3;
        in_b = 0; in_c = 0;//кол-во элементов в соответствующих файлах
        f1.open(a, ios::in); f2.open(b, ios::out); f3.open(c, ios::out);
        comp++;
        while (ii < n) {  //перезаписываем блоки из а поочередно в b и с
            comp += 2;
            if (bcfile) { //запись в b 
                comp++;
                for (int i = 0; i < r_block_size; i++) {
                    comp++;
                    f1 >> buf;
                    f2 << buf; in_b++;
                    ii++;
                    comp++;
                    if (ii >= n) break;
                    rw += 2;
                }
                bcfile = false;
            }
            else {//запись в с
                comp++;
                for (int i = 0; i < r_block_size; i++) {
                    comp += 2;
                    f1 >> buf;
                    f3 << buf; in_c++;
                    ii++;
                    if (ii >= n) break;
                    rw += 2;
                }
                bcfile = true;
            }
        }
        f1.close(); f2.close(); f3.close();
        //этап слияния
        //f1.open(a, ios::out); f2.open(b, ios::in); f3.open(c, ios::in);
        //int kol_it = max(in_b, in_c);
        //kol_it = min(in_b, in_c) / r_block_size + 1; //кол-во блоков, которых необходимо слить
        //mov += 1; comp++;
        //for (int i = 0; i < kol_it; i++) {
        //    int cur_por_left_f2 = 0, cur_por_left_f3 = 0; //сколько элементов в блоке осталось
        //    //если размер блока меньше остатка в файле то остаток = размер блока
        //    //иначе это оставшееся кол-во в файле
        //    mov += 4; comp += 2;
        //    if (r_block_size <= in_b) cur_por_left_f2 = r_block_size;
        //    else cur_por_left_f2 = in_b;
        //    if (r_block_size <= in_c) cur_por_left_f3 = r_block_size;
        //    else cur_por_left_f3 = in_c;
        //    comp++;
        //    while (cur_por_left_f2 > 0 && cur_por_left_f3 > 0) {//пока оба блока не пусты
        //        //читаем элемент, если блок не пустой и из этого файла нужно прочитать следующий 
        //        //элемент
        //        comp += 3;
        //        if (need_to_read_b && in_b > 0) f2 >> buf;
        //        if (need_to_read_c && in_c > 0) f3 >> buf2;
        //        comp++;
        //        if (buf.name.compare(buf2.name) < 0) {//сравниваем и меньший записываем в f1
        //            f1 << buf; rw++; mov += 2;
        //            cur_por_left_f2--; in_b--;
        //            comp += 2; mov += 2;
        //            if (cur_por_left_f2) {//в каком файле нужно прочитать следующее число
        //                need_to_read_b = true; need_to_read_c = false;
        //            }
        //            else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
        //                need_to_read_b = true;
        //                need_to_read_c = true;
        //            }
        //        }
        //        else {
        //            f1 << buf2; rw++; mov += 2;
        //            cur_por_left_f3--; in_c--;
        //            comp += 2; mov += 2;
        //            if (cur_por_left_f3) {//в каком файле нужно прочитать следующее число
        //                need_to_read_b = false; need_to_read_c = true;
        //            }
        //            else if (cur_por_left_f2 == 0 && cur_por_left_f3 == 0) {
        //                need_to_read_b = true;
        //                need_to_read_c = true;
        //            }
        //        }
        //    }
        //    comp++;
        //    if (in_b == 0 && in_c > 0) {//если файл закончился, то остаток другого переливаем
        //        comp++;
        //        while (in_c > 0) {
        //            comp++; mov++; rw += 2;
        //            in_c--;
        //            if (r_block_size == 1) {
        //                f3 >> buf2;
        //                f1 << buf2;
        //            }
        //            else {
        //                f1 << buf2;
        //                f3 >> buf2;
        //            }
        //        }
        //    }
        //    comp++;
        //    if (in_c == 0 && in_b > 0) {//если файл закончился, то остаток другого переливаем
        //        comp++;
        //        while (in_b > 0) {
        //            comp++; mov++; rw += 2;
        //            in_b--;
        //            if (r_block_size == 1) {
        //                f2 >> buf;
        //                f1 << buf;
        //            }
        //            else {
        //                f1 << buf;
        //                f2 >> buf;
        //            }
        //        }
        //    }
        //    //после while один из блоков пуст, поэтому переливаем в f1 остаток другого блока
        //    comp += 2;
        //    if (in_b > 0 && cur_por_left_f2 > 0) {
        //        comp++;
        //        while (cur_por_left_f2 > 0) {
        //            comp++; rw++; mov += 2;
        //            f1 << buf; in_b--;
        //            cur_por_left_f2--;
        //            comp++;
        //            if (in_b > 0 && cur_por_left_f2) { f2 >> buf; rw++; }
        //        }
        //        mov += 2;
        //        need_to_read_b = true; need_to_read_c = true;
        //    }
        //    else if (in_c > 0 && cur_por_left_f3 > 0) {
        //        comp++;
        //        while (cur_por_left_f3 > 0) {
        //            comp++; rw++; mov += 2;
        //            f1 << buf2; in_c--;
        //            cur_por_left_f3--;
        //            comp++;
        //            if (in_c > 0 && cur_por_left_f3) {
        //                f3 >> buf2; rw++;
        //            }
        //        }
        //        mov += 2;
        //        need_to_read_b = true; need_to_read_c = true;
        //    }
        //}
        //comp++;
        //need_to_read_b = true; need_to_read_c = true;
        //r_block_size *= 2;//увеличиваем размер блока в 2 раза
        //f1.close(); f2.close(); f3.close();
        merge(f1, f2, f3, a, b, c, buf, buf2, in_b, in_c, r_block_size, need_to_read_b, need_to_read_c);
    }
}
int main()
{
    string osn, d1="b.txt", d2="c.txt";
    int kol, as;
    cout << "Enter main file name" << endl;
    cin >> osn;
    cout << "Enter num of elements" << endl;
    cin >> kol;

    cout << "Enter array size" << endl;
    cin >> as;
    mov = 0; comp = 0; rw = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    hybrid_mergesort(osn, d1, d2,kol, as);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Runnig time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    std::cout << "Runnig time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mics]" << std::endl;
    cout << "comparison = " << comp << endl;
    cout << "inner memory mov = " << mov << endl;
    cout << "file accesses = " << rw << endl;
    cout << "Total = " << rw+mov+comp<< endl;

}

