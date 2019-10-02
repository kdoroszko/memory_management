#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

class Node
{
public:
    Node(const int v) :
        next(nullptr),
        value(v)
    {}

    /*~Node(){
        cout << "Hello from destructor :)" << '\n';
        delete next;
    }*/

    //Node* next;
    shared_ptr<Node> next;
    int value;
};

class EmptyListError : public runtime_error
{
public:
    EmptyListError(const std::string& what_arg);
};

EmptyListError::EmptyListError(const std::string& what_arg) : runtime_error(what_arg){}

class List
{
public:
    List();
    //void add(Node* node);
    void add(shared_ptr<Node> node);
    void add_unique(unique_ptr<Node> node);
    void add_at_front(shared_ptr<Node> node);
    //Node* get(const int value);
    shared_ptr<Node> get(const int value);
    shared_ptr<Node> get_reverse(const int value);
    void clear();

    //~List(){ delete first;}

private:
    //Node* first;
    shared_ptr<Node> first;
    shared_ptr<Node> last;

    shared_ptr<Node> get_reverse(const int value, shared_ptr<Node> new_current);
    bool it_is_not_the_last_node(shared_ptr<Node>& node);
    bool this_node_has_not_a_next(shared_ptr<Node>& node);
    bool it_is_not_the_node_from_list(shared_ptr<Node>& node);
    void display_error_message_cant_add_twice_same_node(shared_ptr<Node>& node);
};

List::List() :
    first(nullptr), last(nullptr)
{}

void List::add(/*Node**/ shared_ptr<Node> node)
{
    if(!first)
    {
        first = node;
        last = node;
    }
    else if(it_is_not_the_node_from_list(node))
        {
            //Node* current = first;
            auto current = first;

            while(current->next)
            {
                current = current->next;
            }

            current->next = node;
            last = node;
        }
        else display_error_message_cant_add_twice_same_node(node);
}

void List::add_unique(unique_ptr<Node> node)
{
    shared_ptr<Node> shared = move(node);

    if(!first)
    {
        first = shared;
        last = shared;
    }
    else
    {
        auto current = first;

        while(current->next)
        {
            current = current->next;
        }

        current->next = shared;
        last = shared;
    }
}

void List::add_at_front(shared_ptr<Node> node)
{
    if(!first)
    {
        first = node;
        last = node;
    }
    else if(it_is_not_the_node_from_list(node))
        {
            auto temporary_ptr_to_node(first);
            
            first = node;
            node->next = temporary_ptr_to_node;
        }
        else display_error_message_cant_add_twice_same_node(node);
}

//Node* List::get(const int value)
shared_ptr<Node> List::get(const int value)
{
    if(!first)
    {
        //cout << "List is empty!" << '\n';
        //return nullptr;

        //throw runtime_error("List is empty!");
        throw EmptyListError("List is empty!");
    }
    else
    {
        //Node* current = first;
        auto current = first;
        do
        {
            if(current->value == value)
            {
                cout << "Found value " << current->value << '\n';
                return current;
            }
            else
            {
                cout << "Going through " << current->value << '\n';
                current = current->next;
            }
        } while(current);
        cout << "Not found: value " << value << '\n';
        return nullptr;
    }
}

shared_ptr<Node> List::get_reverse(const int value)
{
    if(!first)
        throw EmptyListError("List is empty!");
    else
    {
        auto current = first;

        return get_reverse(value, current);
    }
}

void List::clear()
{
    first = nullptr;
    last = nullptr;
}

shared_ptr<Node> List::get_reverse(const int value, shared_ptr<Node> new_current)
{
    shared_ptr<Node> result;

    if(new_current->next)
        result = get_reverse(value, new_current->next);
    if(!result && new_current->value == value)
        result = new_current;

    return result;
}

bool List::it_is_not_the_last_node(shared_ptr<Node>& node)
{
    return node != last;
}

bool List::this_node_has_not_a_next(shared_ptr<Node>& node)
{
    return !node->next;
}

bool List::it_is_not_the_node_from_list(shared_ptr<Node>& node)
{
    return it_is_not_the_last_node(node) && this_node_has_not_a_next(node);
}

void List::display_error_message_cant_add_twice_same_node(shared_ptr<Node>& node)
{
    cout << "You can't add same node twice! (Node value: " << node->value << ")\n";
}

int main()
{

    try
    {
        List lista;
        //Node* node4 = new Node(4);
        //Node* node7 = new Node(7);
        //auto node_error_test = lista.get(1);

        auto node4 = make_shared<Node>(4);
        auto node7 = make_shared<Node>(7);
        auto node3 = make_shared<Node>(3);

        lista.add(node4);
        lista.add(make_shared<Node>(2));
        lista.add(node7);
        lista.add(make_shared<Node>(9));
        lista.add(node3);
        lista.add(node3);

        auto node12 = make_shared<Node>(12);
        lista.add_at_front(node12);
        lista.add_at_front(node12);
        lista.add_at_front(node7);
        lista.add_at_front(node3);

        auto node = lista.get(1);
        node = lista.get_reverse(1);

        lista.clear();
        auto test_node = make_unique<Node>(2);
        //lista.add_unique(test_node);
        lista.add_unique(make_unique<Node>(3));
        lista.add_unique(make_unique<Node>(2));
        lista.add_unique(make_unique<Node>(1));
        lista.get(1);

        lista.clear();
        node = lista.get(1);

        if (node)
            cout << node->value << '\n';
    }

    catch(const EmptyListError& e)
    {
        cerr << "EmptyListError: " << e.what() << '\n';
    }

    /*catch(const runtime_error& e)
    {
        cerr << "Runtime_error: " << e.what() << '\n';
    }*/

    return 0;
}
