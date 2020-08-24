#include <bits/stdc++.h>
#include "cprint.h"
using namespace std;

#define WORKPATH ".\\"

typedef unsigned short uint16;
typedef unsigned long long uint64;

uint64 current_minute() {
  return chrono::time_point_cast<chrono::minutes>(chrono::system_clock::now()).time_since_epoch().count();
}

const double error_ratio = 1.02566539646643248114;
const int priority_ratio = 100;
const int version = 1001;

const double e = exp(1);
const double _ln10 = 1.0 / log(10);

mt19937_64 rng;

uint64 randuint(uint64 l, uint64 r) {
  return rng() % (r - l + 1) + l;
}

void put_string(ofstream& os, string s) {
  int len = s.length();
  os.write((char*) &len, sizeof(len));
  auto tmp = s.c_str();
  os.write(tmp, len);
}
string read_string(ifstream& os) {
  int len;
  os.read((char*) &len, sizeof(len));
  char *tmp = new char[len + 1];
  tmp[len] = 0;
  os.read(tmp, len);
  string ret = tmp;
  delete[] tmp;
  return ret;
}

typedef class Word {
  public:
    string key;
    string val;
    uint16 err_msk;
    uint64 first_correct_time;
    uint64 last_correct_time;

    Word() {  }
    Word(string key, string val) : key(key), val(val), err_msk(0u), first_correct_time(0u), last_correct_time(0u) { }

    int error_num() {
      return __builtin_popcount(err_msk);
    }
    int priority() {
      if (!last_correct_time) {
        return priority_ratio * pow(error_ratio, error_num()) + 0.1;
      }
//      return max(1, (int) (priority_ratio * 0.5 * (0.56 * pow((current_minute() - last_correct_time) * 1.0 / 60, 0.06)) * pow(error_ratio, error_num()) + 0.1));
      double coef = exp(-((last_correct_time - first_correct_time) * 1.0 / 8640));
      double rem = 1.0 / (log((current_minute() - last_correct_time) * coef + e) * _ln10);
      double val = priority_ratio * 0.7 * (1 - rem) * pow(error_ratio, error_num());
      val = max(val, 1.0);
      return (int) (val + 0.1);
    }

    bool judge(string ans) {
      bool res = ans != key;
      err_msk = err_msk << 1 | res;
      res = !res;
      if (res) {
        last_correct_time = current_minute();
      }
      if (!first_correct_time && last_correct_time) {
        first_correct_time = last_correct_time;
      }
      return res;
    }

    void writein(ofstream& os) {
      put_string(os, key);
      put_string(os, val);
      os.write((char*) &err_msk, sizeof(err_msk));
      os.write((char*) &first_correct_time, sizeof(first_correct_time));
      os.write((char*) &last_correct_time, sizeof(last_correct_time));
    }
    void readin(ifstream& os, int ver = ::version) {
      key = read_string(os);  
      val = read_string(os);
      os.read((char*) &err_msk, sizeof(err_msk));
      if (ver >= 1001) os.read((char*) &first_correct_time, sizeof(first_correct_time));
      os.read((char*) &last_correct_time, sizeof(last_correct_time));
      if (ver < 1001) first_correct_time = last_correct_time;
    }

    bool operator < (Word b) const {
      return (key != b.key) ? key < b.key : val < b.val;
    }
} Word;

typedef class WordContainer {
  public:
    set<Word> S;

    bool insert(Word w) {
      if (S.count(w)) {
        return false;
      }
      S.insert(w);
    }
    void update(Word w) {
      if (S.count(w)) {
        S.erase(w);
      }
      S.insert(w);
    }

    void erase(Word w) {
      if (S.count(w)) {
        S.erase(w);
      }
    }

    vector<Word> to_list() {
      vector<Word> ret;
      ret.reserve(S.size());
      for (auto& w : S) {
        ret.push_back(w);
      }
      return ret;
    }

    size_t size() {
      return S.size();
    }
} WordContainer;

bool save_flag = false;
WordContainer G;

void create_home_directory() {
  // TODO
}

void boot() {
  // TODO
}

Word get_random_word() {
  auto v = G.to_list();
  vector<Word> v0;
  vector<uint64> pv;
  for (auto& x : v) {
    int y = x.priority();
    pv.push_back(y);
    if (y >= priority_ratio) {
      v0.push_back(x);
    }
  }
  if (!v0.empty()) {
    v = v0;
    pv.clear();
    for (auto& x : v) {
      int y = x.priority();
      pv.push_back(y);
    }
  }
  uint64 sum = 0;
  for (auto& x : pv) {
    sum += x;
    x = sum;
  }
  uint64 x = randuint(1, sum);
  int t = lower_bound(pv.begin(), pv.end(), x) - pv.begin();
  return v[t];
}

string get_command(string s) {
  char *tmp = new char[s.length() + 1];
  sscanf(s.c_str(), "%s", tmp);
  return tmp;
}

void clear() {
  system("cls");
}
void pause() {
  system("pause");
}

string current_list;

void import(string name) {
  current_list = name;
  string filename (string(WORKPATH".reciteword\\") + name + ".txt");
  ifstream fin (filename);
  if (!fin) {
    cprintf("[错误]", fontcolor::Red | fontcolor::Intensity);
    cout << " 打开文件失败" << endl;
    pause();
    return;
  }
  string a, b;
  while (fin) {
    getline(fin, a);
    getline(fin, b);
    if (!a.length() || !b.length()) {
      continue;
    }
    G.update(Word(a, b));
    cerr << a << " " << b << '\n';
  }
}
void save0() {
  using namespace fontcolor;
  string filename (string(WORKPATH".reciteword\\") + current_list + ".txt");
  ofstream fout (filename);
  if (!fout) {
    cprintf("[错误]", Red | Intensity);
    cout << " 保存失败，请检查目录和文件" << endl;
    pause();
    return;
  }
  for (auto& w : G.S) {
    fout << w.key << '\n';
    fout << w.val << '\n';
  }
  fout.close();
}

void load(string name) {
  using namespace fontcolor;
  string filename (string(WORKPATH".reciteword\\") + name + ".dat");
  ifstream fin (filename, ios::binary);
  if (!fin) {
    cprintf("[错误]", Red | Intensity);
    cout << " 文件不存在" << endl;
    pause();
    return;
  }
  int version;
  fin.read((char*) &version, sizeof(version));
  if (version != ::version) {
    cprintf("[警告]", Red | Green | Intensity);
    cout << " 主程序版本不同，这个版本可能已经不受到支持了" << endl;
  }
  int num;
  fin.read((char*) &num, sizeof(num));
  Word w;
  while (num > 0) {
    w.readin(fin, version);
    G.update(w);
    num--;
  }
}
void save(string name) {
  using namespace fontcolor;
  string filename (string(WORKPATH".reciteword\\") + name + ".dat");
  ofstream fout (filename, ios::binary);
  if (!fout) {
    cprintf("[错误]", Red | Intensity);
    cout << " 保存失败，请检查目录和文件" << endl;
    pause();
    return;
  }
  fout.write((char*) &version, sizeof(version));
  int sz = G.size();
  fout.write((char*) &sz, sizeof(sz));
  auto list = G.to_list();
  for (auto& w : list) {
    w.writein(fout);
  }
  save_flag = true;
}

void do_recite() {
  using namespace fontcolor;
  if (!G.size()) {
    cprintf("[错误]", Red | Intensity);
    cout << " 找不到单词" << endl;
    pause();
    return;
  }
  int num = min((int) (G.size() * 0.4), 100);
  num = max(num, 5);
  int ac = 0;
  for (int t = 1; t <= num; t++) {
    clear();
    putchar('[');
    for (int i = 1; i <= 100; i++) {
      cprintf("%c", ((ac * 100 >= num * i) ? Green : White) | Intensity, ".#"[(t - 1) * 100 >= num * i]);
    }
    putchar(']');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    fflush(stdout);

    auto w = get_random_word();
    cout << w.val << endl;
    string x;
    getline(cin, x);

    bool res = w.judge(x);
    G.update(w);
    if (res) {
      cprintf("Accepted", fontcolor::Green | Intensity);
      ac++;
    } else {
      cprintf("Wrong Answer", fontcolor::Red | Intensity);
      cout << ", 正确答案: " << w.key;
    }
    cout << endl;
    pause();
  }
}

void debug_out() {
  for (auto x : G.S) {
    cerr << "{" << "key: " << x.key << ", val: " << x.val << " , msk: " << x.err_msk << ", last time: " << x.last_correct_time << ", first time: " << x.first_correct_time << ", priority = " << x.priority() << "}\n";
  }
}

int main() {
  using namespace fontcolor;
  boot();
  string lines;
  while (true) {
    clear();
    try {
      cout << ">>> ";
      getline(cin, lines);
      string cm = get_command(lines);
      if (cm == "import") {
        try {
          import(lines.substr(7));
          pause();
          save_flag = false;
        } catch (out_of_range) {
          cprintf("[错误] ", Red | Intensity);
          cout << "请检查输入，用法：import [文件主名]" << endl;
          pause();
        }
      } else if (cm == "save0") {
        save0();
      } else if (cm == "save") {
        try {
          save(lines.substr(5));
        } catch (out_of_range) {
          cprintf("[错误] ", Red | Intensity);
          cout << "请检查输入，用法：save [文件主名]" << endl;
          pause();
        }
      } else if (cm == "load") {
        try {
          load(lines.substr(5));
          save_flag = false;
        } catch (out_of_range) {
          cprintf("[错误] ", Red | Intensity);
          cout << "请检查输入，用法：load [文件主名]" << endl;
          pause();
        }
      } else if (cm == "exit") {
        if (!save_flag) {
          cout << "你还没有保存，是否确定退出？(Y/N) ";
          char c;
          cin >> c;
          if (c == 'Y') {
            return 0;
          }
        } else {
          return 0;
        }
      } else if (cm == "set") {
        string a, b;
        cout << "单词 ";
        getline(cin, a);
        cout << "含义 ";
        getline(cin, b);
        G.update(Word(a, b));
        save_flag = false;
      } else if (cm == "recite") {
        do_recite();
        save_flag = false;
      } else if (cm == "dbg") {
        debug_out();
        pause();
      } else if (cm == "remove") {
        string a, b;
        cout << "单词 ";
        getline(cin, a);
        cout << "含义 ";
        getline(cin, b);
        G.erase(Word(a, b));
        save_flag = false;
      }
    } catch (exception e) {
      cprintf("[错误] ", Red | Intensity);
      cerr << e.what() << endl;
    }
  }
  return 0;
}
