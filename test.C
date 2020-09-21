#include <iostream>
#include <thread>

class hist
{
public:
    TH1F *h1;

    hist(Int_t n);
    ~hist();
    void Loop(int n)
    {
        for (int i = 0; i < n; i++)
        {
            cout << i << endl;
        }
    }
};

hist::hist(Int_t n)
{
    h1 = new TH1F("h1", "TITLE", 10, 0, 10);
}

hist::~hist()
{
    cout << "Dest called" << endl;
    if (h1)
    {
        cout << "delete h1" << endl;
        h1->Delete();
    }
}

void test()
{
    hist h(3);
    thread th1(h.Loop, 10);
    th1.join();
}