#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
};

template <typename T>
class LinkedList {
private:
    Node<T>* head_;
    size_t size_;
public:
    LinkedList() : head_(nullptr), size_(0) {}
    LinkedList(const LinkedList& other) : head_(nullptr), size_(0) {
        if (other.head_ != nullptr) {
            Node<T>* current = other.head_;
            do {
                push_tail(current->data);
                current = current->next;
            } while (current != other.head_);
        }
    }

    ~LinkedList() {
        clear();
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList temp(other);
            std::swap(head_, temp.head_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }

    void clear() {
        while (size_ > 0) {
            pop_head();
        }
    }

    size_t size() const { return size_; }

    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>{value, nullptr, nullptr};
        if (head_ == nullptr) {
            head_ = newNode;
            head_->next = head_;
            head_->prev = head_;
        } else {
            Node<T>* tail = head_->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head_;
            head_->prev = newNode;
        }
        size_++;
    }

    void push_head(const T& value) {
        push_tail(value);
        head_ = head_->prev;
    }

    void pop_head() {
        if (head_ == nullptr) throw std::underflow_error("List is empty");
        Node<T>* toDelete = head_;
        if (size_ == 1) {
            head_ = nullptr;
        } else {
            Node<T>* tail = head_->prev;
            head_ = head_->next;
            tail->next = head_;
            head_->prev = tail;
        }
        delete toDelete;
        size_--;
    }

    void pop_tail() {
        if (head_ == nullptr) throw std::underflow_error("List is empty");
        if (size_ == 1) {
            delete head_;
            head_ = nullptr;
            size_ = 0;
            return;
        }
        Node<T>* tail = head_->prev;
        Node<T>* newTail = tail->prev;
        
        newTail->next = head_;
        head_->prev = newTail;
        
        delete tail;
        size_--;
    }

    const T& operator[](size_t index) const {
        if (head_ == nullptr) throw std::out_of_range("List is empty");
        if (index >= size_) index %= size_;
        
        Node<T>* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T& operator[](size_t index) {
        if (head_ == nullptr) throw std::out_of_range("List is empty");
        if (index >= size_) index %= size_;
        
        Node<T>* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
        os << "[";
        if (list.head_ != nullptr) {
            Node<T>* current = list.head_;
            for (size_t i = 0; i < list.size_; ++i) {
                os << current->data;
                if (i < list.size_ - 1) os << " <-> ";
                current = current->next;
            }
        }
        os << "]";
        return os;
    }

    void fill_random(size_t count, T min_val, T max_val, unsigned int seed = 42) {
        clear();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(static_cast<int>(min_val), static_cast<int>(max_val));
        for (size_t i = 0; i < count; ++i) {
            push_tail(static_cast<T>(dist(gen)));
        }
    }
};

template <typename T>
LinkedList<T> merge_sorted_lists(const LinkedList<T>& list1, const LinkedList<T>& list2) {
    LinkedList<T> result;
    size_t i = 0;
    size_t j = 0;
    size_t n1 = list1.size();
    size_t n2 = list2.size();
    while (i < n1 && j < n2) {
        T val1 = list1[i];
        T val2 = list2[j];
        if (val1 < val2) {
            if (result.size() == 0 || result[result.size() - 1] != val1) {
                result.push_tail(val1);
            }
            i++;
        } else if (val1 > val2) {
            if (result.size() == 0 || result[result.size() - 1] != val2) {
                result.push_tail(val2);
            }
            j++;
        } else {
            if (result.size() == 0 || result[result.size() - 1] != val1) {
                result.push_tail(val1);
            }
            i++;
            j++;
        }
    }

    while (i < n1) {
        T val = list1[i];
        if (result.size() == 0 || result[result.size() - 1] != val) {
            result.push_tail(val);
        }
        i++;
    }
    while (j < n2) {
        T val = list2[j];
        if (result.size() == 0 || result[result.size() - 1] != val) {
            result.push_tail(val);
        }
        j++;
    }
    return result;
}

int main() {
    try {
        std::cout << "--- Lab 2: Doubly Circular Linked List (Variant 4) ---\n\n";
        LinkedList<int> list1;
        LinkedList<int> list2;
        list1.push_tail(1); list1.push_tail(3); list1.push_tail(5); list1.push_tail(5); list1.push_tail(7);
        list2.push_tail(2); list2.push_tail(3); list2.push_tail(4); list2.push_tail(6); list2.push_tail(8);
        std::cout << "List 1: " << list1 << "\n";
        std::cout << "List 2: " << list2 << "\n\n";
        LinkedList<int> merged = merge_sorted_lists(list1, list2);
        std::cout << "Merged sorted list (without duplicates): \n";
        std::cout << merged << "\n\n";
        LinkedList<int> copyList = list1;
        std::cout << "Copy of List 1: " << copyList << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
