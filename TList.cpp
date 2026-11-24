#include "TList.h"

#include <iomanip>
#include <istream>
#include <ostream>
#include <utility>

using namespace std;

bool StudentList::read(istream& input) {
    int count = 0;
    if (!(input >> count) || count < 0) {
        return false;
    }
    for (int i = 0; i < count; ++i) {
        Student student;
        if (!(input >> quoted(student.fullName) >> student.course
                    >> student.group >> student.studyForm)) {
            return false;
        }

        student.firstExam = exams_.size();
        for (size_t j = 0; j < examCount; ++j) {
            Exam exam;
            if (!(input >> quoted(exam.subject) >> exam.grade)) {
                return false;
            }
            exams_.push_back(move(exam));
        }
        addToTail(move(student));
    }
    return true;
}

int StudentList::countDebts(const Student& student) const noexcept {
    int debts = 0;
    for (size_t i = 0; i < examCount; ++i) {
        if (exams_[student.firstExam + i].grade == 2) {
            ++debts;
        }
    }
    return debts;
}

void StudentList::processSession(ostream& output) {
    Node* previous = nullptr;
    Node* current = head_;
    while (current != nullptr) {
        auto* next = current->next;
        auto& student = current->value;
        const int debts = countDebts(student);

        if (debts >= 3) {
            output << student.fullName << " — отчисление с " << student.course
                   << "-го курса (не сдано " << debts << " из " << examCount << " экзаменов)\n";
            removeAfter(previous);
        } else if (debts == 0 && (student.course == 4 || student.course == 6)) {
            output << student.fullName << " — выпуск с " << student.course << "-го курса\n";
            removeAfter(previous);
        } else {
            if (debts == 0) {
                output << student.fullName << " — перевод с " << student.course
                       << "-го на " << student.course + 1 << "-й курс\n";
                ++student.course;
            }
            previous = current;
        }
        current = next;
    }
}

void StudentList::printList(ostream& output) const {
    output << size() << '\n';
    for (const auto* node = head_; node != nullptr; node = node->next) {
        const auto& student = node->value;
        output << quoted(student.fullName) << ' ' << student.course << ' '
               << student.group << ' ' << student.studyForm << '\n';
        for (size_t i = 0; i < examCount; ++i) {
            const auto& exam = exams_[student.firstExam + i];
            output << quoted(exam.subject) << ' ' << exam.grade << '\n';
        }
    }
}
