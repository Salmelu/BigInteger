/**
 * Třída pro práci s velkými čísly. Čísla jsou ukládána jako pole proměnných typu integer,
 *  které je vyplňeno jednotlivými číslicemi velkého čísla. Využívá schopnosti procesoru pracovat
 *  rychle s 32-bitovými čísly a zároveň téměř nulové nutnosti konverze čísel a čísla ukládá v soustavě
 *  se základem 10^9 a ve stejné so ustavě s nimi počítá. 
 * Tato třída umí provádět operace sčítání, odčítání, násobení, dělení, modulo,
 *  porovnávání a výpis čísla nebo převod do objektu string.
 * 
 * @author Michal Staruch
 * @version 1.0 13/12/2013 
 */
#ifndef BIGINTEGER_HPP
#define	BIGINTEGER_HPP

namespace bigInt {
	class BigInteger;
}

class bigInt::BigInteger {
public:
	/**
	 * Konstruktor přijímající celočíselný typ integer jako parametr, vytvoří nový objekt typu BigInteger
	 * 
	 * @param	n	proměnná typu integer použitá k vytvoření objektu (pokud není uvedena, vytvoří se objekt BigInteger s číselnou hodnotou 0)
	 */
    BigInteger(int n=0);
	
	/**
	 * Konstruktor přijímající celočíselný typ long jako parametr, vytvoří nový objekt typu BigInteger
	 * 
	 * @param	n	proměnná typu long použitá k vytvoření objektu
	 */
    BigInteger(long n);
	
	/**
	 * Konstruktor přijímající řetězec typu C (pole typu char). Konstruktor použije metodu k převedení tohoto 
	 * pole na třídu string standartní knihovny jazyka C++ a zavolá jejich společnou metodu pro vytvoření objektu BigInteger
	 * 
	 * @param	s	ukazatel na pole typu pro vytvoření objektu typu BigInteger 
	 * (pokud je předán nulový ukazatel, nebo objekt s neplatnými znaky (jakékoliv znaky kromě znaku - a číslic),
	 *  vytvoří se objekt BigInteger s číselnou hodnotou 0)
	 */
	BigInteger(const char* s);
	
	/**
	 * Konstruktor přijímající instanci objektu string standartní knihovny jazyka C++ a zavolá metodu pro vytvoření objektu BigInteger
	 * 
	 * @param	s	instance objektu string
	 * (pokud je předán řetězec s neplatnými znaky, vytvoří se objekt BigInteger s číselnou hodnotou 0)
	 */
    BigInteger(std::string const& s);
    
	/**
	 * Konstruktor, který zkopíruje objekt a naalokuje novou paměť pro čísla nového objektu 
	 * (tento konstruktor je nutný, jinak by ukazatel nového objektu ukazoval na stejnou paměť.)
	 * 
     * @param	source	zdrojový objekt, ze kterého je pořízena kopie
     */
	BigInteger(const bigInt::BigInteger &source);
	
	
	/**
	* Přičte k volajícímu objektu BigInteger předaný parametr a vrátí ukazatel na volající objekt 
	*
	* @param	a	objekt BigInteger, který se má přičíst k volajícímu objektu
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger add(bigInt::BigInteger a);
	
	/**
	* Přetížení operátoru +, operace se chová jako standartní sčítání. Původní objekt se nezmění,
	*  namísto toho metoda vrací ukazatel na nový objekt obsahující výsledek.
	*
	* @param	a	odkaz na objekt BigInteger, který se má přičíst k volajícímu objektu
	* @return      odkaz na nový objekt obsahující součet volaného objektu a parametru
	*/
    bigInt::BigInteger operator+ (bigInt::BigInteger const &a);
	
	/**
	* Přetížení operátoru +=, operace se chová jako standartní sčítání a přiřazení.
	*  Hodnota původního objektu se přepíše součtem původního objektu a parametru.
	*  Tato metoda pouze volá metodu add.
	*
	* @param	a	odkaz na objekt BigInteger, který se má přičíst k volajícímu objektu
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger operator+= (bigInt::BigInteger const &a);
    
	
	
	
	/**
	* Odečte od volajícího objektu BigInteger předaný parametr a vrátí ukazatel na volající objekt 
	*
	* @param	s	objekt BigInteger, který se má odečíst od volajícího objektu
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger subtract(bigInt::BigInteger s);
	
	/**
	* Přetížení operátoru -, operace se chová jako standartní odčítání. Původní objekt se nezmění,
	*  namísto toho metoda vrací ukazatel na nový objekt obsahující výsledek.
	*
	* @param	s	odkaz na objekt BigInteger, který se má odečíst od volajícího objektu
	* @return      odkaz na nový objekt obsahující rozdíl volaného objektu a parametru
	*/
    bigInt::BigInteger operator- (bigInt::BigInteger const &s);
	
	/**
	* Přetížení operátoru -=, operace se chová jako standartní odčítání a přiřazení.
	*  Hodnota původního objektu se přepíše rozdílem původního objektu a parametru.
	*  Tato metoda pouze volá metodu subtract.
	*
	* @param	s	odkaz na objekt BigInteger, který se má odečíst od volajícího objektu
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger operator-= (bigInt::BigInteger const &s);
	
	
	
	
	/**
	 * Vynásobí volající objekt BigInteger předaným parametrem a vrátí ukazatel na volající objekt
	 * 
     * @param	b	odkaz na objekt BigInteger, kterým bude vynásoben volající objekt
     * @return		odkaz na volající objekt
     */
	bigInt::BigInteger multiply(bigInt::BigInteger b);
	
	/**
	* Přetížení operátoru *, operace se chová jako standartní násobení. Původní objekt se nezmění,
	*  namísto toho metoda vrací ukazatel na nový objekt obsahující výsledek.
	*
	* @param	b	odkaz na objekt BigInteger, kterým se volající objekt vynásobí
	* @return      odkaz na nový objekt obsahující součin volaného objektu a parametru
     */
    bigInt::BigInteger operator* (bigInt::BigInteger const &b);
	
	/**
	* Přetížení operátoru *=, operace se chová jako standartní násobení a přiřazení.
	*  Hodnota původního objektu se přepíše součinem původního objektu a parametru.
	*  Tato metoda pouze volá metodu multiply.
	*
	* @param	b	odkaz na objekt BigInteger, kterým se volající objekt vynásobí
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger operator*= (bigInt::BigInteger const &b);
	
	
	
	
	/**
	 * Vydělí volající objekt BigInteger předaným parametrem a vrátí ukazatel na volající objekt
	 * 
     * @param	b	odkaz na objekt BigInteger, kterým bude vydělen volající objekt
     * @return		odkaz na volající objekt
     */
	bigInt::BigInteger divide(bigInt::BigInteger b);
	
	/**
	* Přetížení operátoru /, operace se chová jako standartní dělení. Původní objekt se nezmění,
	*  namísto toho metoda vrací ukazatel na nový objekt obsahující výsledek.
	*
	* @param	b	odkaz na objekt BigInteger, kterým se volající objekt vydělí
	* @return      odkaz na nový objekt obsahující podíl volaného objektu a parametru
     */
    bigInt::BigInteger operator/ (bigInt::BigInteger const &b);
	
	/**
	* Přetížení operátoru /=, operace se chová jako standartní dělení a přiřazení.
	*  Hodnota původního objektu se přepíše podílem původního objektu a parametru.
	*  Tato metoda pouze volá metodu divide.
	*
	* @param	b	odkaz na objekt BigInteger, kterým se volající objekt vydělí
	* @return      odkaz na volající objekt
	*/
    bigInt::BigInteger operator/= (bigInt::BigInteger const &b);
	
	
	
	
	/**
	 * Provede operaci modulo na volajícím objektu typu BigInteger a vrátí odkaz na volající objekt
	 * 
     * @param	b	odkaz na objekt BigInteger, kterým bude provedena operace modulo
     * @return		odkaz na volající objekt
     */
	bigInt::BigInteger modulo(bigInt::BigInteger b);
	
	/**
	* Přetížení operátoru %, operace se chová jako standartní operace modulo. Původní objekt se nezmění,
	*  namísto toho metoda vrací ukazatel na nový objekt obsahující výsledek.
	*
	* @param	b	odkaz na objekt BigInteger, kterým bude provedena operace modulo
	* @return      odkaz na nový objekt obsahující modulo volajícího objektu parametrem
     */
    bigInt::BigInteger operator% (bigInt::BigInteger const &b);
	
	/**
	* Přetížení operátoru %=, operace se chová jako standartní modulo a přiřazení.
	*  Hodnota původního objektu se přepíše výsledkem operace modulo volajícího objektu parametrem.
	*  Tato metoda pouze volá metodu modulo.
	*
	* @param	b	odkaz na objekt BigInteger, kterým bude provedena operace modulo
	* @return      odkaz na volající objekt
     */
    bigInt::BigInteger operator%= (bigInt::BigInteger const &b);
    
	
	
	
	/**
	 * Porovná číselné hodnoty 2 objektů typu BigInteger a vrátí číslo vyjadřující 
	 * výsledek jejich porovnání.
	 * 
     * @param	second	odkaz na objekt typu BigInteger, který bude porovnán s volajícím objektem
     * @return			vrátí -1, pokud je volající objekt menší než parametr, 
	 * 1, pokud je volající objekt větší než parametr a 0, pokud mají objekty stejnou číselnou hodnotu
     */
    int compare (bigInt::BigInteger const &second);
	
	/**
	 * Přetížení operátoru &lt. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající objekt menší, než parametr, false v ostatních případech
     */
    bool operator< (bigInt::BigInteger const &c);
	
	/**
	 * Přetížení operátoru &gt. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající objekt větší, než parametr, false v ostatních případech
     */
    bool operator> (bigInt::BigInteger const &c);
	
	/**
	 * Přetížení operátoru &lt=. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající objekt menší nebo roven parametru, false v ostatních případech
     */
    bool operator<= (bigInt::BigInteger const &c);
	
	/**
	 * Přetížení operátoru &gt=. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající objekt větší nebo roven parametru, false v ostatních případech
     */
    bool operator>= (bigInt::BigInteger const &c);
	
	/**
	 * Přetížení operátoru ==. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající roven parametru, false v ostatních případech
     */
    bool operator== (bigInt::BigInteger const &c);
	
	/**
	 * Přetížení operátoru !=. Porovná objekty pomocí metody compare a vytřídí výsledky.
	 * 
     * @param	c	odkaz na BigInteger, se kterým se má volající objekt porovnat
     * @return		true, pokud je volající objekt rozdílný od parametru, false, pokud je stelný
     */
    bool operator!= (bigInt::BigInteger const &c);
    
	
	
	
	/**
	 * Vypíše volající objekt na standartní výstup. 
	 * K výpisu je nejprve objekt převeden na string a poté vypsán
     */
    void print();
	
	/**
	 * Převede objekt BigInteger na objekt string standartní knihovny jazyka C++
	 * 
     * @return		objekt typu string obsahující hodnotu objektu BigInteger
     */
    std::string toString();
	
	/**
	 * Přetížení přetypovacího operátoru. Převede objekt na objekt 
	 * string metodou toString a vrátí na odkaz na tento objekt.
	 * 
     * @return		objekt typu string obsahující hodnotu objektu BigInteger
     */
    operator std::string();
    
private:
    int *number;		// pointer pointing to the memory where number is stored
    int nsize;			// number size
    int asize;			// memory allocated size
    static const int base = 1000000000;
    bool sign;          // true if number is negative
	
	void stringInit(std::string const &s);		// initializes number from string or char*
    
    bool alloc(int n);							// allocates memory for the number
	void recount();								// recounts the size of a number for/after an operation
    std::string getLeadingZeros(int i);		// adds leading zeros for printing number
    bool checkString(std::string const &s);	// checks if a string is valid number
    int parseString(std::string const &s);		// parses string into a BigInteger
	void shiftLeft(int len);					// shifts the number len spaces to left
	bigInt::BigInteger simpleDivision(int d, bool mod=false);	// divides a number by integer
	bigInt::BigInteger division(bigInt::BigInteger d, bool mod=false); // divides a number - this method is hidden because
								// a different methods are public and this one is called by those methods
};

#endif	/* BIGINTEGER_HPP */
