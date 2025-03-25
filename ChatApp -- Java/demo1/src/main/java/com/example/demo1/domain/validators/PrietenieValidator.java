package com.example.demo1.domain.validators;

import com.example.demo1.domain.Prietenie;
import com.example.demo1.domain.Tuple;
import com.example.demo1.repository.Repository;
import com.example.demo1.repository.database.PrietenieDbRepository;
import com.example.demo1.repository.database.UtilizatorDbRepository;
import com.example.demo1.repository.file.PrietenieRepository;
import com.example.demo1.repository.file.UtilizatorRepository;

public class PrietenieValidator implements Validator<Prietenie> {
    @Override
    public void validate(Prietenie prietenie) {
        String erori = "";
        if(prietenie.getId1() == prietenie.getId2()){
            erori += "invalid friendship";

        }
        if(prietenie.getDate() == null )
            erori += "invalid date";
        if(!erori.equals(""))
        {
            throw new ValidationException(erori);
        }
    }
    public static void validare_cu_repo(Prietenie prietenie, PrietenieDbRepository repo2, UtilizatorDbRepository repo) {
        String erori = "";
        if(repo.findOne(prietenie.getId1()).isEmpty())
            erori += "invalid id";
        if(repo.findOne(prietenie.getId2()).isEmpty())
            erori += "invalid id";
        if(repo2.findOne(new Tuple<Long, Long>(prietenie.getId1(), prietenie.getId2())).isPresent())
        {
            erori += "deja prieteni";
        }
        if(prietenie == null)
            erori += "invalid friendship";
        if(!erori.isEmpty())
            throw new ValidationException(erori);

    }
}
