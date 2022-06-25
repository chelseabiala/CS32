class Investment
{
    public:
        Investment(string n, int p);
        virtual ~Investment();
        string name() const;
        int purchasePrice() const;
        virtual string description() const = 0;
        virtual bool fungible() const;
    private:
        string m_name;
        int m_price;
};

Investment::Investment(string n, int p)
: m_name(n), m_price(p)
{}

Investment::~Investment()
{}

string Investment::name() const
{
    return m_name;
}

int Investment::purchasePrice() const
{
    return m_price;
}

bool Investment::fungible() const
{
    return false;
}

class Painting : public Investment
{
    public:
        Painting(string n, int p);
        ~Painting();
        virtual string description() const;
    private:
};

Painting::Painting(string n, int p)
: Investment(n, p)
{}

Painting::~Painting()
{
    cout << "Destroying " << name() << ", a painting" << endl;
}

string Painting::description() const
{
    return "painting";
}

class Stock : public Investment
{
    public:
        Stock(string n, int p, string d);
        ~Stock();
        virtual string description() const;
        virtual bool fungible() const;
    private:
        string m_description;
};

Stock::Stock(string n, int p, string d)
: Investment(n, p), m_description(d)
{}

Stock::~Stock()
{
    cout << "Destroying " << name() << ", a stock holding" << endl;
}

string Stock::description() const
{
    string c = "stock trading as ";
    c += m_description;
    return c;
}

bool Stock::fungible() const
{
    return true;
}

class House : public Investment
{
    public:
        House(string n, int p);
        ~House();
        virtual string description() const;
    private:
};

House::House(string n, int p)
: Investment(n, p)
{}

House::~House()
{
    cout << "Destroying the house " << name() << endl;
}

string House::description() const
{
    return "house";
}
