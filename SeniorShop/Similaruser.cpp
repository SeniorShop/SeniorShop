#include "Similaruser.h"

void SimilarUser::set_name(const std::string &set_name) {
    username = set_name;
}
void SimilarUser::set_password(const std::string &set_password) {
    password = set_password;
}
void SimilarUser::set_status(const std::string &set_status) {
    status = set_status;
}
std::string SimilarUser::get_name() const {
    return username;
}
std::string SimilarUser::get_password() const {
    return password;
}
std::string SimilarUser::get_status() const {
    return status;
}
