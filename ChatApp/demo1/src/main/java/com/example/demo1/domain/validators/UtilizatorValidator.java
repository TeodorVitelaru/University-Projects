package com.example.demo1.domain.validators;


import com.example.demo1.domain.Utilizator;

public class UtilizatorValidator implements Validator<Utilizator> {
    @Override
    public void validate(Utilizator entity) throws ValidationException {
        //TODO: implement method validate
        if(entity.getFirstName().equals("") || entity.getLastName().equals(""))
            throw new ValidationException("Utilizatorul nu este valid");
    }
}
