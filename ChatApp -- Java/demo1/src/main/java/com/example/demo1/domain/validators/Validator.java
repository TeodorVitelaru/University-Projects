package com.example.demo1.domain.validators;

public interface Validator<T> {
    void validate(T entity) throws ValidationException;
}