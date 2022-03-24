#include <iostream>
#include <fstream>
#include <string>
using namespace std;
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
void mergesort(string a, string b, string c,int n) {
    fstream f1, f2, f3;
    toy buf,buf2;
    bool bcfile = true;
    bool need_to_read_b = true, need_to_read_c = true;
    int r_block_size = 1, in_b=0, in_c=0;
    while (r_block_size < n) {
        int ii = 0;
        in_b = 0; in_c = 0;
        f1.open(a, ios::in); f2.open(b, ios::out); f3.open(c, ios::out);
        while (ii < n) {
            if (bcfile) {
                for (int i = 0; i < r_block_size; i++) {
                    f1 >> buf;
                    f2 << buf; in_b++;
                    ii++;
                    if (ii >= n) break;
                    bcfile = false;
                }
            }
            else {
                for (int i = 0; i < r_block_size; i++) {
                    f1 >> buf;
                    f3 << buf; in_c++;
                    ii++;
                    if (ii >= n) break;
                    bcfile = true;
                }
            } 
        }
        f1.close(); f2.close(); f3.close();

        f1.open(a, ios::out); f2.open(b, ios::in); f3.open(c, ios::in);
        int kol_it = max(in_b, in_c);
        for (int i = 0; i < kol_it; i++) {
            int cur_por_left_f2 = r_block_size, cur_por_left_f3 = r_block_size;
            while (cur_por_left_f2 > 0 && cur_por_left_f3 > 0) {
                if(need_to_read_b && in_b >0) f2 >> buf;
                if(need_to_read_c && in_c>0) f3 >> buf2;

                if (in_b > 0 && in_c > 0) {
                    if (buf.price < buf2.price) {
                        f1 << buf;
                        cur_por_left_f2--; in_b--;
                        if (in_b > 0 && cur_por_left_f2) {
                            f2 >> buf;
                            need_to_read_b = false; need_to_read_c = false;
                        }
                    }
                    else {
                        f1 << buf2;
                        cur_por_left_f3--; in_c--;
                        if (in_c > 0 && cur_por_left_f3) {
                            f3 >> buf2;
                            need_to_read_b = false; need_to_read_c = false;
                        }
                    }
                }
                else {
                    if (in_b > 0) {
                        f1 << buf;
                        cur_por_left_f2--; in_b--;
                        if (in_b > 0 && cur_por_left_f2) f2 >> buf;
                    }
                    else {
                        f1 << buf2;
                        cur_por_left_f3--; in_c--;
                        if (in_c > 0 && cur_por_left_f3) f3 >> buf2;
                    }
                }
            }
            if (in_b > 0 && cur_por_left_f2 > 0) {
                while (cur_por_left_f2 > 0) {
                    f1 << buf; in_b--;
                    cur_por_left_f2--;
                    if (in_b > 0 && cur_por_left_f2) f2 >> buf;
                }
                need_to_read_b = true; need_to_read_c = true;
            }
            else if(in_c > 0&& cur_por_left_f3 > 0){
                while (cur_por_left_f3 > 0) {
                    f1 << buf2; in_c--;
                    cur_por_left_f3--;
                    if (in_c > 0 && cur_por_left_f3) f3 >> buf2;
                }
                need_to_read_b = true; need_to_read_c = true;
            }
        }
        r_block_size *= 2;
        f1.close(); f2.close(); f3.close();
    }
}
int main()
{
    string osn, d1="b.txt", d2="c.txt";
    int kol;
    cout << "Enter main file name" << endl;
    cin >> osn;
    cout << "Enter num of elements" << endl;
    cin >> kol;
    mergesort(osn, d1, d2,kol);

}

