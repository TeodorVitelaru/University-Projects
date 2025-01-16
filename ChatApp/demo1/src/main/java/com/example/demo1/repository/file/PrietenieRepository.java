package com.example.demo1.repository.file;

import com.example.demo1.domain.Prietenie;
import com.example.demo1.domain.Tuple;
import com.example.demo1.domain.validators.Validator;


import java.time.LocalDate;
import java.util.Arrays;
import java.util.List;

public class PrietenieRepository extends AbstractFileRepository<Tuple<Long, Long>, Prietenie> {

    public PrietenieRepository(String filename, Validator<Prietenie> validator) {
        super(validator, filename);
    }

    @Override
    public Prietenie createEntity(String line) {
        List<String> values = Arrays.asList(line.split(";"));
        long id1 = Long.parseLong(values.get(0));
        long id2 = Long.parseLong(values.get(1));
        LocalDate date = LocalDate.parse(values.get(2));
        Prietenie prietenie = new Prietenie(id1, id2, date);
        prietenie.setId(new Tuple<>(id1, id2));

        return prietenie;
   }


    @Override
    public String saveEntity(Prietenie entity) {
        return entity.getId1() + ";" + entity.getId2() + ";" + entity.getDate();
    }
}
