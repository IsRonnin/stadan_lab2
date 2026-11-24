#pragma once

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class LinkedList {
public:
    virtual ~LinkedList() = 0;
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    std::size_t size() const noexcept { return size_; }

protected:
    struct Node {
        T value;
        Node* next = nullptr;
    };

    LinkedList() = default;

    void addToTail(T value) {
        auto* node = new Node{std::move(value)};
        if (head_ == nullptr) {
            head_ = node;
        } else {
            tail_->next = node;
        }
        tail_ = node;
        ++size_;
    }

    void removeAfter(Node* previous) noexcept {
        auto* node = previous == nullptr ? head_ : previous->next;
        if (previous == nullptr) {
            head_ = node->next;
        } else {
            previous->next = node->next;
        }
        if (node == tail_) {
            tail_ = previous;
        }
        delete node;
        --size_;
    }

    Node* head_ = nullptr;

private:
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

template <typename T>
LinkedList<T>::~LinkedList() {
    while (head_ != nullptr) {
        removeAfter(nullptr);
    }
}

struct Exam {
    std::string subject;
    int grade;
};

struct Student {
    std::string fullName;
    int course;
    std::string group;
    std::string studyForm;
    std::size_t firstExam;
};

class StudentList : public LinkedList<Student> {
public:
    bool read(std::istream& input);
    void printList(std::ostream& output) const;
    void processSession(std::ostream& output);

private:
    int countDebts(const Student& student) const noexcept;

    static constexpr std::size_t examCount = 5;
    std::vector<Exam> exams_;
};
