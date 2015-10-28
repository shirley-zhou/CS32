/*
#include <iostream>
#include <string>
using namespace std;
*/

class Character
{
public:
	Character(string name);
	virtual ~Character() {};

	string name() const;
	virtual void printWeapon() const=0;
	virtual string attackAction() const;
private:
	string m_name;
};

class Dwarf : public Character
{
public:
	Dwarf(string name);
	virtual ~Dwarf();
	virtual void printWeapon() const;
	//virtual string attackAction() const;
};

class Elf : public Character
{
public:
	Elf(string name, int arrows);
	virtual ~Elf();
	virtual void printWeapon() const;
	//virtual string attackAction() const;
private:
	int m_arrows;
};

class Boggie : public Character
{
public:
	Boggie(string name);
	virtual ~Boggie();
	virtual void printWeapon() const;
	virtual string attackAction() const;
};

//implementations:

Character::Character(string name)
	:m_name(name)
{}

string Character::name() const
{
	return m_name;
}

string Character::attackAction() const
{
	return "rushes toward the enemy";
}

Dwarf::Dwarf(string name)
	:Character(name)
{}

Dwarf::~Dwarf()
{
	cout << "Destroying " << name() << " the dwarf" << endl;
}

void Dwarf::printWeapon() const
{
	cout << "an axe";
}

Elf::Elf(string name, int arrows)
	:Character(name), m_arrows(arrows)
{}

Elf::~Elf()
{
	cout << "Destroying " << name() << " the elf" << endl;
}

void Elf::printWeapon() const
{
	cout << "a bow and quiver of " << m_arrows << " arrows";
}

Boggie::Boggie(string name)
	:Character(name)
{}

Boggie::~Boggie()
{
	cout << "Destroying " << name() << " the boggie" << endl;
}

void Boggie::printWeapon() const
{
	cout << "a short sword";
}

string Boggie::attackAction() const
{
	return "whimpers";
}
/*
void strike(const Character* cp)
{
    cout << cp->name() << ", wielding ";
    cp->printWeapon();
    cout << ", " << cp->attackAction() << "." << endl;
}

int main()
{
    Character* characters[4];
    characters[0] = new Dwarf("Gimlet");
        // Elves have a name and initial number of arrows in their quiver
    characters[1] = new Elf("Legolam", 10);
    characters[2] = new Boggie("Frito");
    characters[3] = new Boggie("Spam");

    cout << "The characters strike!" << endl;
    for (int k = 0; k < 4; k++)
        strike(characters[k]);

        // Clean up the characters before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
	delete characters[k];
}
*/