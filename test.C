class hist
{
public:
    std::vector<std::string> luminosity_labels;

    hist(){};
    ~hist(){};
};

void test()
{
    map<int, string> v;
    hist *a = new hist();
    a->luminosity_labels = {"test", "hallo"};
    for (const auto &i : a->luminosity_labels)
    {
        cout << i << endl;
    }
}