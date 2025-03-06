#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits> 
using namespace std; 


// Global IDs 
int id_of_book = 10001;
int id_of_user = 20001;

int get_valid_int() {
    int num;
    bool first_attempt = true;
    while (true) {
        if (cin >> num) {
            return num;
        } else {
           
            if (!first_attempt) {
                cout << "Invalid input. Please enter a valid number: ";
            }
            first_attempt = false;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


enum class user_type {  student, faculty, librarian };

enum class status { available, borrowed, reserved};
    
    

class Book {
private:
    int id_book;
    string title;
    string author;
    string publisher;
    int year;
    string isbn;
    status book_status;


public:
    Book(){}
    Book(int id, const string &t, const string &a, const string &p, int y, const string &isbn_val, status s)
        : id_book(id), title(t), author(a), publisher(p), year(y), isbn(isbn_val), book_status(s){}

    void set_id(int id){
        id_book = id;
    }
    int get_id() const{
        return id_book;
    }

    void set_title(const string &t){
        title = t;
    }
    string get_title() const{
        return title;
    }

    void set_author(const string &a){
        author = a;
    }
    string get_author() const{
        return author;
    }

    void set_publisher(const string &p){
        publisher = p;
    }
    string get_publisher() const{
        return publisher;
    }

    void set_year(int y){
        year = y;
    }
    int get_year() const{
        return year;
    }


    void set_isbn(const string &i){
        isbn = i;
    }
    string get_isbn() const{
        return isbn;
    }


    void set_status(status s){
        book_status = s;
    }
    status get_status() const{
        return book_status;
    }

    
    string to_string() const{
        ostringstream oss;
        oss << id_book << "|" << title << "|" << author << "|" << publisher << "|"
            << year << "|" << isbn << "|" << static_cast<int>(book_status);
        return oss.str();
    }

    
    static Book from_string(const string &str){
        Book b;
        istringstream iss(str);
        string token;

        getline(iss, token, '|');
        b.id_book = stoi(token);

        getline(iss, b.title, '|');
        getline(iss, b.author, '|');
        getline(iss, b.publisher, '|');

        getline(iss, token, '|');
        b.year = stoi(token);

        getline(iss, b.isbn, '|');
        getline(iss, token, '|');
        b.book_status = static_cast<status>(stoi(token));

        return b;
    }
};

class Library;


class User {
protected:
    string name;
    string username;
    string password;
    user_type user_type_var;
    int id_user;

public:
    User() {
        id_user = id_of_user;
        id_of_user++;
    }
    int get_user_id() const{
        return id_user;
    }


    void set_name(){
        cout << "Enter name: ";
        getline(cin >> ws, name); 
    }
    string get_name() const{
        return name;
    }


    void set_username(){
        cout << "Enter username:";
        cin >> username;
    }
    string get_username() const{
        return username;  
    }

    user_type get_user_type() const{
        return user_type_var; 
    }
    void set_user_type(user_type ut){
        user_type_var = ut;  
    }


    void set_password(){
        string pass1, pass2;
        cout << "Enter password:";
        cin >> pass1;
        cout << "Enter password again:";
        cin >> pass2;


        if (pass1 == pass2){
            password = pass1;
        }
        else{
            cout << "Passwords do not match. Please try again." << endl;
            set_password();
        }
    }

    string get_password() const{
        return password;
    }

    virtual void create_account(){
        set_name();
        set_username();
        set_password();
    }
    
    virtual void show_menu(Library &lib) = 0; // this is pure virtual so that menus are shown differently to different users


    // direct setters 
    // these are needed because i need to initialise them
    void set_name_direct(const string &n) {
        name = n;
    }

    void set_username_direct(const string &u) {
        username = u;
    }

    void set_password_direct(const string &p) {
        password = p;
    }


};


struct BorrowRecord {
    Book book;
    time_t borrow_date;
    time_t return_date;
};

class Account {

private:
    vector< pair<Book, time_t> > borrowed_books;
    vector<BorrowRecord> history;


public:

    const vector< pair<Book, time_t> > &get_borrowed_books() const{
        return borrowed_books;
    }

    int get_fines(){
        int total_fine = 0;
        time_t now = time(0);

        for (auto &entry : borrowed_books){
            int days = difftime(now, entry.second) / (60 * 60 * 24); 

            if (days>15){
                total_fine += ((days - 15) * 10);
            }
        }
        return total_fine;
    }

    void view_fines(){
        int total_fine = 0;
        time_t now = time(0);

        for (auto &entry : borrowed_books){
            int days = difftime(now, entry.second) / (60 * 60 * 24);

            if (days > 15) {
                int fine = ((days - 15) * 10);
                cout << "Book: " << entry.first.get_title() << " Fine: Rs " << fine << endl;
                total_fine += fine;
            }

            else {
                cout << "Book: " << entry.first.get_title() << " has no pending fine" << endl;
            }
        }

        cout << "Total fine: Rs " << total_fine << endl;
    }



    void add_borrowed_book(const Book &book){
        time_t now = time(0);
        borrowed_books.push_back({book, now});
    }


    void return_borrowed_book(int book_id){

        for (auto it = borrowed_books.begin(); it != borrowed_books.end(); it++){
            if (it->first.get_id() == book_id) {
                time_t return_time = time(0);
                BorrowRecord record;
                record.book = it->first;
                record.borrow_date = it->second;
                record.return_date = return_time;
                history.push_back(record);
                borrowed_books.erase(it);
                cout << "Book ID " << book_id << " returned successfully." << endl;
                return;
            }
        }
        

        cout << "Book ID " << book_id << " not found in borrowed list." << endl;
    }

    void show_history(){
        if(history.empty()){
            cout << "No borrowing history." << endl;
            return;
        }
        for(auto &record : history){
            cout << "Book: " << record.book.get_title() 
                 << " | Borrowed on: " << ctime(&record.borrow_date)
                 << " | Returned on: " << ctime(&record.return_date);
        }
    }
    

    void pay_fines(){
        int fine = get_fines();
        if (fine == 0){
            cout << "No fines to pay." << endl;
            return;
        }

        cout << "Total fine: Rs " << fine << endl;
        cout << "Press 1 to pay fines: ";
        int inp = get_valid_int();

        if (inp == 1){
            cout << "Fine payment done..." << endl;
        }
    }
};


class Student : public User
{
private:
    int max_books;
    int num_books_borrowed;
    int max_period;
    Account account;

public:
    Student() : max_books(3), num_books_borrowed(0), max_period(15){
        set_user_type(user_type::student);
    }

    void create_account() override{
        cout << "\n--- Creating student account ---" << endl;
        User::create_account();
    }

    void borrow_book(vector<Book> &books){
        
        if (account.get_fines() > 0){
            cout << "Clear pending fines to borrow a book." << endl;
            return;
        }
        if (num_books_borrowed >= max_books){
            cout << "Borrowing limit already reached." << endl;
            return;
        }

        cout << "Enter book ID to borrow:" << endl;
        int book_id = get_valid_int();

        for (auto &book : books){
            if (book.get_id() == book_id){

                if (book.get_status() == status::available){
                    book.set_status(status::borrowed);
                    account.add_borrowed_book(book);
                    num_books_borrowed++;
                    cout << "Book - " << book.get_title() << " borrowed successfully." << endl;
                    return;
                }
                else
                {
                    cout << "Book is not available." << endl;
                    return;
                }
            }
        }

        cout << "No book with this Book ID exists in the library." << endl;
    }


    void return_book(vector<Book> &books){
        cout << "Enter book ID to return: ";
        int book_id = get_valid_int();

        account.return_borrowed_book(book_id);

        for (auto &book : books){
            if (book.get_id() == book_id){

                book.set_status(status::available);
                num_books_borrowed--;
                return;
            }
        }
    }

    Account &get_account(){
        return account;
    }

    string serialize() {
        ostringstream oss;
        
        oss << get_user_id() << "|" << name << "|" << username << "|" << password << "|" 
            << max_books << "|" << num_books_borrowed << "|" << max_period;
        return oss.str();
    }

    static Student* deserialize(const string &line) {
        Student* s = new Student();
        istringstream iss(line);
        string token;
        
        getline(iss, token, '|');
        s->id_user = stoi(token); 
        getline(iss, s->name, '|');
        getline(iss, s->username, '|');
        getline(iss, s->password, '|');
        getline(iss, token, '|');
        s->max_books = stoi(token);
        getline(iss, token, '|');
        s->num_books_borrowed = stoi(token);
        getline(iss, token, '|');
        s->max_period = stoi(token);
        return s;
    }


    void show_menu(Library &lib) override;
};





class Faculty : public User
{
private:
    int max_books;
    int num_books_borrowed;
    int max_period;
    Account account;

public:
    Faculty() : max_books(5), num_books_borrowed(0), max_period(30)
    {
        set_user_type(user_type::faculty);
    }

    void create_account() override
    {
        cout << "\n--- Creating faculty account ---" << endl;
        User::create_account();
    }

    void borrow_book(vector<Book> &books)
    {
        if (num_books_borrowed >= max_books)
        {
            cout << "Borrowing limit reached." << endl;
            return;
        }

        cout << "Enter book ID to borrow: ";
        int book_id = get_valid_int();

        for (auto &book : books)
        {
            if (book.get_id() == book_id)
            {
                if (book.get_status() == status::available)
                {
                    book.set_status(status::borrowed);
                    account.add_borrowed_book(book);
                    num_books_borrowed++;
                    cout << "Book \"" << book.get_title() << "\" borrowed successfully." << endl;
                    return;
                }
                else
                {
                    cout << "Book is not available." << endl;
                    return;
                }
            }
        }

        cout << "Book ID not found." << endl;
    }

    void return_book(vector<Book> &books)
    {
        cout << "Enter book ID to return: ";
        int book_id = get_valid_int();

        account.return_borrowed_book(book_id);

        for (auto &book : books)
        {
            if (book.get_id() == book_id)
            {
                book.set_status(status::available);
                num_books_borrowed--;
                return;
            }
        }
    }

    Account &get_account(){
        return account;
    }

    string serialize() {
        ostringstream oss;
        oss << get_user_id() << "|" << name << "|" << username << "|" << password << "|" 
            << max_books << "|" << num_books_borrowed << "|" << max_period;
        return oss.str();
    }

    static Faculty* deserialize(const string &line) {
        Faculty* f = new Faculty();
        istringstream iss(line);
        string token;
        getline(iss, token, '|');
        f->id_user = stoi(token);
        getline(iss, f->name, '|');
        getline(iss, f->username, '|');
        getline(iss, f->password, '|');
        getline(iss, token, '|');
        f->max_books = stoi(token);
        getline(iss, token, '|');
        f->num_books_borrowed = stoi(token);
        getline(iss, token, '|');
        f->max_period = stoi(token);
        return f;
    }


    void show_menu(Library &lib) override;
};



class Librarian : public User{
public:
    Librarian(){
        set_user_type(user_type::librarian);
    }

    void create_account() override{
        User::create_account();
        cout << "\n--- Creating librarian account ---" << endl;
    }


    void add_new_book(vector<Book>& books) {
        Book book;
        string title, author, publisher, isbn;
        int year;

        cout << "Enter title: ";
        getline(cin >> ws, title); 

        cout << "Enter author: ";
        getline(cin >> ws, author);

        cout << "Enter publisher: ";
        getline(cin >> ws, publisher);

        cout << "Enter publication year: ";
        year = get_valid_int();

        cout << "Enter isbn: ";
        getline(cin >> ws, isbn);

        book.set_title(title);
        book.set_author(author);
        book.set_publisher(publisher);
        book.set_year(year);
        book.set_isbn(isbn);
        book.set_status(status::available);
        book.set_id(id_of_book);
        id_of_book++;
        books.push_back(book);

        cout << "Book added successfully with ID " << book.get_id() << endl;
    }


    void update_book(vector<Book> &books){
        cout << "Enter book ID to update: ";
        int book_id = get_valid_int();

        for (auto &book : books){
            if (book.get_id() == book_id){
                
               
                cout << "1. Update title"<<endl << "2. Update author"<<endl <<"3. Update publisher"<<endl <<"4. Update year"<<endl <<"5. Update isbn"<<endl;
                cout << "Enter your choice: " << endl;
                int input = get_valid_int();
                cin.ignore(); // clear newline

                switch (input){
                    case 1:{
                        cout << "Enter new title: ";
                        string new_title;
                        getline(cin, new_title);
                        book.set_title(new_title);
                        break;
                    }
                    case 2:{
                    
                        cout << "Enter new author: ";
                        string new_author;
                        getline(cin, new_author);
                        book.set_author(new_author);
                        break;
                    }
                    case 3:{
                    
                        cout << "Enter new publisher: ";
                        string new_publisher;
                        getline(cin, new_publisher);
                        book.set_publisher(new_publisher);
                        break;
                    }

                    case 4:{
                    
                        cout << "Enter new year: ";
                        
                        int new_year = get_valid_int();
                        book.set_year(new_year);
                        break;
                    }

                    case 5:{
                    
                        cout << "Enter new isbn: ";
                        string new_isbn;
                        cin >> new_isbn;
                        book.set_isbn(new_isbn);
                        break;
                    }
                    default:
                        cout << "Invalid choice." << endl;
                }

                cout << "Book updated successfully." << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }


    void remove_book(vector<Book> &books){
        cout << "Enter book ID to remove: ";
        int book_id = get_valid_int();

        for (auto it = books.begin(); it != books.end(); it++) {
            if (it->get_id() == book_id){

                books.erase(it);
                cout << "Book removed successfully." << endl;
                return;
            }
        }

        cout << "Book not found." << endl;
    }

    string serialize() {
        ostringstream oss;
        // Librarian might have fewer extra fields
        oss << get_user_id() << "|" << name << "|" << username << "|" << password;
        return oss.str();
    }

    static Librarian* deserialize(const string &line) {
        Librarian* l = new Librarian();
        istringstream iss(line);
        string token;
        getline(iss, token, '|');
        l->id_user = stoi(token);
        getline(iss, l->name, '|');
        getline(iss, l->username, '|');
        getline(iss, l->password, '|');
        return l;
    }


    void add_new_user(Library &lib);
    void remove_user(Library &lib) ;
    void view_history(Library &lib) ;
    void show_menu(Library &lib) override;

};





class Library {
private:
    vector<Book> books_list;
    vector<Student *> student_list;
    vector<Faculty *> faculty_list;
    vector<Librarian *> librarian_list;

    
    string user_name_setter() {

        string name;
        cout << "Enter username: ";
        cin >> name;
        return name;
    }
   
    bool re_enter_password(const string &pass) {
        cout << "Enter password again: ";
        string pw;
        cin >> pw;
        return (pw == pass);
    }


public:
    vector<Book> &all_books() {
        return books_list;
    }
    vector<Book> available_books() {
        vector<Book> avail;
        for (auto &b : books_list)
        {
            if (b.get_status() == status::available)
                avail.push_back(b);
        }
        return avail;
    }


    void add_new_student(Student *s){
        student_list.push_back(s);
    }

    void add_new_faculty(Faculty *f){
        faculty_list.push_back(f);
    }

    void add_new_librarian(Librarian *l){
        librarian_list.push_back(l);
    }

    int get_student_count() const{
        return student_list.size();
    }

    int get_faculty_count() const{
        return faculty_list.size();
    }

    int get_librarian_count() const {
        return librarian_list.size();
    }

    
    friend class Librarian;

    string select_login_type(){
        cout << "\nWelcome to People's Library" << endl;
        cout << "Press number for Login Type" << endl;
        cout << "1. Student" << endl;
        cout << "2. Librarian" << endl;
        cout << "3. Faculty" << endl;
        int type = get_valid_int();

        if (type == 1){
            return "Student";
        }

        else if (type == 2){
            return "Librarian";

        }
        else if (type == 3){
            return "Faculty";
        }

        else{
            cout << "Invalid selection. Try again." << endl;
            return select_login_type();
        }
    }

    void view_all_books() {
        if (books_list.empty()) {
            cout << "No books available in the library." << endl;
            return;
        }
        

        cout << left << setw(10) << "Book ID" 
             << setw(30) << "Title" 
             << setw(20) << "Author" 
             << setw(25) << "Publication" 
             << setw(10) << "Year" 
             << setw(15) << "Status" << endl;
        


        cout << string(110, '-') << endl;
        

        for (const auto &book : books_list) {
            cout << left << setw(10) << book.get_id()
                 << setw(30) << book.get_title()
                 << setw(20) << book.get_author()
                 << setw(25) << book.get_publisher()
                 << setw(10) << book.get_year();
            

            if (book.get_status() == status::available)
                cout << setw(15) << "Available";
            else
                cout << setw(15) << "Not Available";
            
            cout << endl;
        }
    }


    void welcome(){
        string login_type_str = select_login_type();
        string uname;
        bool found = false;
        User *current_user = nullptr;

        while (true){
            uname = user_name_setter();
            found = false;

            if (login_type_str == "Student"){
                for (Student *s : student_list) {

                    if (s->get_username() == uname){
                        found = true;
                        current_user = s;
                        break;
                    }
                }
            }


            else if (login_type_str == "Faculty"){
                for (Faculty *f : faculty_list) {

                    if (f->get_username() == uname)   {
                        found = true;
                        current_user = f;
                        break;
                    }
                }
            }


            else if (login_type_str == "Librarian")    {
                for (Librarian *l : librarian_list){


                    if (l->get_username() == uname)
                    {
                        found = true;
                        current_user = l;
                        break;
                    }
                }
            }


            if (!found){
                cout << "No " << login_type_str << " with username " << uname << " found. Please try again." << endl;
                continue;
            }

            cout << "Enter password: ";
            string pass;
            cin >> pass;

            if (current_user->get_password() == pass){
                cout << "Login successful! Welcome, " << uname << "!" << endl;
                break;
            }
            else {
                cout << "Wrong password entered." << endl;
                bool valid = re_enter_password(current_user->get_password());
                while (!valid)
                {
                    valid = re_enter_password(current_user->get_password());
                }
                cout << "Login successful! Welcome, " << uname << "!" << endl;
                break;
            }
        }

        current_user->show_menu(*this);
    }
    


    void load_books(const string &filename) {
        ifstream file(filename);
        if (file.is_open()){
            
            string header;
            getline(file, header);
            
            string line;
            while(getline(file, line)){
                if(!line.empty()){
                    Book b = Book::from_string(line);
                    books_list.push_back(b);
                    if(b.get_id() >= id_of_book)
                        id_of_book = b.get_id() + 1;
                }
            }
            file.close();
        }
}



    void save_books(const string &filename) {
        ofstream file(filename);
        if (file.is_open()){
            file << "id|title|author|publisher|year|isbn|status" << "\n";
            for (auto &b : books_list) {
                file << b.to_string() << "\n";
            }
            file.close();
        }
    }
    

    void save_students(const string &filename) {
        ofstream file(filename);
        if (file.is_open()){
          
            file << "id|name|username|password|max_books|num_books_borrowed|max_period" << "\n";
            for (auto s : student_list) {
                file << s->serialize() << "\n";
            }
            file.close();
        }
    }
    
    

    void load_students(const string &filename) {
        ifstream file(filename);
        if (file.is_open()){
           
            string header;
            getline(file, header);
            
            string line;
            while(getline(file, line)){
                if(!line.empty()){
                    Student* s = Student::deserialize(line);
                    student_list.push_back(s);
                }
            }
            file.close();
        }
    }
    
    


    void save_faculty(const string &filename) {
        ofstream file(filename);
        if (file.is_open()){
            file << "id|name|username|password|max_books|num_books_borrowed|max_period" << "\n";
            for (auto f : faculty_list) {
                file << f->serialize() << "\n";
            }
            file.close();
        }
    }
    

    void load_faculty(const string &filename) {
        ifstream file(filename);
        if (file.is_open()){
            // Discard header row
            string header;
            getline(file, header);
            
            string line;
            while(getline(file, line)){
                if(!line.empty()){
                    Faculty* f = Faculty::deserialize(line);
                    faculty_list.push_back(f);
                }
            }
            file.close();
        }
    }
    

    void save_librarians(const string &filename) {
        ofstream file(filename);
        if (file.is_open()){
            
            file << "id|name|username|password" << "\n";
            for (auto l : librarian_list) {
                file << l->serialize() << "\n";
            }
            file.close();
        }
    }
    

    void load_librarians(const string &filename) {
        ifstream file(filename);
        if (file.is_open()){
            
            string header;
            getline(file, header);
            
            string line;
            while(getline(file, line)){
                if(!line.empty()){
                    Librarian* l = Librarian::deserialize(line);
                    librarian_list.push_back(l);
                }
            }
            file.close();
        }
    }
    

    void save_global_ids(const string &filename) {
        ofstream file(filename);
        if (file.is_open()){
           
            file << "id_of_book|id_of_user" << "\n";
            file << id_of_book << "|" << id_of_user;
            file.close();
        }
    }


    void load_global_ids(const string &filename) {
        ifstream file(filename);
        if (file.is_open()){
            
            string header;
            getline(file, header);
          
            string line;
            if(getline(file, line)) {
                istringstream iss(line);
                string token;
                getline(iss, token, '|');
                id_of_book = stoi(token);
                getline(iss, token, '|');
                id_of_user = stoi(token);
            }
            file.close();
        }
    }
    
    

};



void Librarian::add_new_user(Library &lib){
        
    cout << "Add new user:" << endl << "1. Student 2. Faculty 3. Librarian " << endl;
    cout << "Please enter choice :";
    int choice = get_valid_int();

    if (choice == 1){
        Student *s = new Student();
        s->create_account();
        lib.add_new_student(s);
        cout << "Student added successfully with ID " << s->get_user_id() << endl;


    } else if (choice == 2) {
        Faculty *f = new Faculty();
        f->create_account();
        lib.add_new_faculty(f);
        cout << "Faculty added successfully with ID " << f->get_user_id() << endl;

    } else if (choice == 3) {
        Librarian *l = new Librarian();
        l->create_account();
        lib.add_new_librarian(l);
        cout << "Librarian added successfully with ID " << l->get_user_id() << endl;

    }
    else {
        cout << "Invalid choice." << endl;
    }

}



void Librarian::remove_user(Library &lib) {
    cout << "Select user type to remove: 1. Student  2. Faculty  3. Librarian: ";
    int type_choice = get_valid_int();

    cout << "Search by: 1. User ID  2. Username: ";
    int search_choice = get_valid_int();
    bool removed = false;

    if (search_choice == 1) {
        cout << "Enter user ID: ";
        int uid = get_valid_int();

        if (type_choice == 1) {
            for (auto it = lib.student_list.begin(); it != lib.student_list.end(); it++) {
                if ((*it)->get_user_id() == uid) {
                    delete *it;
                    lib.student_list.erase(it);
                    cout << "Student removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else if (type_choice == 2) {
            for (auto it = lib.faculty_list.begin(); it != lib.faculty_list.end(); it++) {
                if ((*it)->get_user_id() == uid) {
                    delete *it;
                    lib.faculty_list.erase(it);
                    cout << "Faculty removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else if (type_choice == 3) {
            for (auto it = lib.librarian_list.begin(); it != lib.librarian_list.end(); it++) {
                if ((*it)->get_user_id() == uid) {
                    delete *it;
                    lib.librarian_list.erase(it);
                    cout << "Librarian removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else {
            cout << "Invalid user type selection." << endl;
            return;
        }
    } else if (search_choice == 2) {
        cout << "Enter username: ";
        string uname;
        getline(cin >> ws, uname);

        if (type_choice == 1) {
            for (auto it = lib.student_list.begin(); it != lib.student_list.end(); it++) {
                if ((*it)->get_username() == uname) {
                    delete *it;
                    lib.student_list.erase(it);
                    cout << "Student removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else if (type_choice == 2) {
            for (auto it = lib.faculty_list.begin(); it != lib.faculty_list.end(); it++) {
                if ((*it)->get_username() == uname) {
                    delete *it;
                    lib.faculty_list.erase(it);
                    cout << "Faculty removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else if (type_choice == 3) {
            for (auto it = lib.librarian_list.begin(); it != lib.librarian_list.end(); it++) {
                if ((*it)->get_username() == uname) {
                    delete *it;
                    lib.librarian_list.erase(it);
                    cout << "Librarian removed successfully." << endl;
                    removed = true;
                    break;
                }
            }
        } else {
            cout << "Invalid user type selection." << endl;
            return;
        }
    } else {
        cout << "Invalid search option." << endl;
        return;
    }

    if (!removed) {
        cout << "User not found." << endl;
    }
}


void Librarian::view_history(Library &lib) {
    cout << "Search by: 1. User ID  2. Username: ";
    int search_choice = get_valid_int();
    bool found = false;

    if (search_choice == 1) {
        cout << "Enter user ID: ";
        int uid = get_valid_int();

        for (auto s : lib.student_list) {
            if (s->get_user_id() == uid) {
                cout << "Borrowing history for student " << s->get_username() << ":" << endl;
                s->get_account().show_history();
                found = true;
                break;
            }
        }
        
        if (!found) {
            for (auto f : lib.faculty_list) {
                if (f->get_user_id() == uid) {
                    cout << "Borrowing history for faculty " << f->get_username() << ":" << endl;
                    f->get_account().show_history();
                    found = true;
                    break;
                }
            }
        }
    } else if (search_choice == 2) {
        cout << "Enter username: ";
        string uname;
        getline(cin >> ws, uname);  

        
        for (auto s : lib.student_list) {
            if (s->get_username() == uname) {
                cout << "Borrowing history for student " << s->get_username() << ":" << endl;
                s->get_account().show_history();
                found = true;
                break;
            }
        }


        if (!found) {
            for (auto f : lib.faculty_list) {
                if (f->get_username() == uname) {
                    cout << "Borrowing history for faculty " << f->get_username() << ":" << endl;
                    f->get_account().show_history();
                    found = true;
                    break;
                }
            }
        }
    } else {
        cout << "Invalid search option." << endl;
        return;
    }

    if (!found) {
        cout << "User not found or no history available." << endl;
    }
}



void Student::show_menu(Library &lib) {
    int choice;
    do {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View all books\n" << "2. Borrow book\n" << "3. Return book\n"
             << "4. View borrowing history\n"
             << "5. View fines\n"
             << "6. Pay fines\n"
             << "7. Logout\n";
        cout << "Enter your choice: ";
        choice = get_valid_int();

        switch (choice) {
            case 1:
                lib.view_all_books();
                break;
            case 2:
                borrow_book(lib.all_books());
                break;
            case 3:
                return_book(lib.all_books());
                break;
            case 4:
                get_account().show_history();
                break;
            case 5:
                get_account().view_fines();
                break;
            case 6:
                get_account().pay_fines();
                break;
            case 7:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 7);
}



void Faculty::show_menu(Library &lib) {
    int choice;
    do {
        cout << "\n--- Faculty Menu ---\n";
        cout << "1. View all books\n"
             << "2. Borrow book\n"
             << "3. Return book\n"
             << "4. View borrowing history\n"
             << "5. Logout\n";
        cout << "Enter your choice: ";
        choice = get_valid_int();

        switch (choice) {
            case 1:
                lib.view_all_books();
                break;
            case 2:
                borrow_book(lib.all_books());
                break;
            case 3:
                return_book(lib.all_books());
                break;
            case 4:
                get_account().show_history();
                break;
            case 5:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 5);
}

void Librarian::show_menu(Library &lib) {
    int choice;
    do {
        cout << "\n--- Librarian Menu ---\n";
        cout << "1. View all books\n"
             << "2. Add new book\n"
             << "3. Update book\n"
             << "4. Remove book\n"
             << "5. Add new user\n"
             << "6. Remove user\n"
             << "7. View user borrowing history\n"
             << "8. Logout\n";
        cout << "Enter your choice: ";
        choice = get_valid_int();

        switch (choice) {
            case 1:
                lib.view_all_books();
                break;
            case 2:
                add_new_book(lib.all_books());
                break;
            case 3:
                update_book(lib.all_books());
                break;
            case 4:
                remove_book(lib.all_books());
                break;
            case 5:
                add_new_user(lib);
                break;
            case 6:
                remove_user(lib);
                break;
            case 7:
                view_history(lib);
                break;
            case 8:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 8);
}



int main(){
    Library lib;

    lib.load_global_ids("global_ids.txt");
    lib.load_books("books.txt");
    lib.load_students("students.txt");
    lib.load_faculty("faculty.txt");
    lib.load_librarians("librarians.txt");


    //10 books
    if (lib.all_books().empty()){
        for (int i = 0; i < 10; i++)
        
        {
            Book b;
            b.set_id(id_of_book);
            id_of_book++;
            b.set_title("Book" + to_string(i + 1));
            b.set_author("Author" + to_string(i + 1));
            b.set_publisher("Publisher" + to_string(i + 1));
            b.set_year(2000 + i);
            b.set_isbn("ISBN" + to_string(i + 1));
            b.set_status(status::available);
            lib.all_books().push_back(b);
            cout << "Book auto-added with ID " << b.get_id() << endl;
        }
    }
        
    //5 students
    if (lib.get_student_count() == 0) {
        for (int i = 0; i < 5; i++) {

            Student* s = new Student();
            s->set_name_direct("Student" + to_string(i + 1));
            s->set_username_direct("student" + to_string(i + 1));
            s->set_password_direct("pass"); 
            lib.add_new_student(s);
            cout << "Student auto-added with ID " << s->get_user_id() << endl;
        }
    }

    // 3 faculty
    if (lib.get_faculty_count() == 0) {

        for (int i = 0; i < 3; i++) {
            
            Faculty* f = new Faculty();
            f->set_name_direct("Faculty" + to_string(i + 1));
            f->set_username_direct("faculty" + to_string(i + 1));
            f->set_password_direct("pass"); 
            lib.add_new_faculty(f);
            cout << "Faculty auto-added with ID " << f->get_user_id() << endl;
        }
    }


    //1 librarian
    if (lib.get_librarian_count() == 0) {

        Librarian* l = new Librarian();

        l->set_name_direct("Librarian");
        l->set_username_direct("librarian");
        l->set_password_direct("pass"); 
        lib.add_new_librarian(l);

        cout << "Librarian auto-added with ID " << l->get_user_id() << endl;
    }



    int main_choice;
    do {
        cout << "--- Library Management System ---" << endl;
        cout << "1. Login\n2. Exit\nEnter your choice number: ";
        main_choice = get_valid_int();

        switch (main_choice){

            case 1:
                lib.welcome();
                break;

            case 2:
                cout << "Exiting system and saving data..." << endl;
                lib.save_books("books.txt");
                lib.save_students("students.txt");
                lib.save_faculty("faculty.txt");
                lib.save_librarians("librarians.txt");
                lib.save_global_ids("global_ids.txt");
                break;


            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (main_choice != 2);

    return 0;
}
