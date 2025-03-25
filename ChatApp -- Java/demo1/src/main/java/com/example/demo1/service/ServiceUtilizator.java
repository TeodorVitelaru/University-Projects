package com.example.demo1.service;

import com.example.demo1.domain.*;
import com.example.demo1.domain.validators.PrietenieValidator;
import com.example.demo1.domain.validators.ValidationException;
import com.example.demo1.repository.database.MessageDbRepository;
import com.example.demo1.repository.database.PrietenieDbRepository;
import com.example.demo1.repository.database.UtilizatorDbRepository;
import com.example.demo1.utils.events.ChangeEventType;
import com.example.demo1.utils.events.EntityChangeEvent;
import com.example.demo1.utils.observer.Observable;
import com.example.demo1.utils.observer.Observer;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static java.lang.Math.*;


public class ServiceUtilizator implements Observable<EntityChangeEvent> {
    private UtilizatorDbRepository repo;
    private PrietenieDbRepository repoPrietenie;
    private MessageDbRepository repoMessage;
    private List<Observer<EntityChangeEvent>> observers = new ArrayList<>();

    public ServiceUtilizator(UtilizatorDbRepository repo, PrietenieDbRepository repoPrietenie, MessageDbRepository repoMessage) {
        this.repo = repo;
        this.repoPrietenie = repoPrietenie;
        this.repoMessage = repoMessage;
    }

    public Page<Utilizator> getUsersOnPage(Pageable pageable) {
       return this.repo.findAllOnPage(pageable);
    }

    @Override
    public void addObserver(Observer<EntityChangeEvent> e) {
        observers.add(e);
    }

    @Override
    public void removeObserver(Observer<EntityChangeEvent> e) {
        observers.remove(e);
    }

    @Override
    public void notifyObservers(EntityChangeEvent t) {
        observers.forEach(x -> x.update(t));
    }

    public void addUtilizator(String nume1, String nume2) {
        try{
            Utilizator user = new Utilizator(nume1, nume2);
            this.repo.save(user);
            EntityChangeEvent event = new EntityChangeEvent(ChangeEventType.ADD, user);
            notifyObservers(event);
        } catch (ValidationException e){
            throw new ValidationException(e.getMessage());
        }
    }

    public Optional<Utilizator> deleteUtilizator(long id) {
        if (repo.findOne(id).isEmpty()) {
            throw new ValidationException("Utilizator negasit");
        }
        try {
            List<Prietenie> prieteniiDeSters = new ArrayList<>();
            this.repoPrietenie.findAll().forEach(prietenie ->{
                if (prietenie.getId1() == id || prietenie.getId2() == id) {
                    prieteniiDeSters.add(prietenie);
                }
            });
            prieteniiDeSters.forEach( prietenie -> {
                this.repoPrietenie.delete(prietenie.getId());
                notifyObservers(new EntityChangeEvent<>(ChangeEventType.DELETE, prietenie));
            });
            Optional<Utilizator> user = this.repo.delete(id);
            if (user.isPresent()) {
                notifyObservers(new EntityChangeEvent<>(ChangeEventType.DELETE, user.get()));
            }
            return user;
        } catch (ValidationException e) {
            throw new ValidationException(e.getMessage());
        }
    }

    public Optional<Utilizator> findOneUser(long id) {
        return this.repo.findOne(id);
    }

    public Iterable<Utilizator> all_utilizators() {
        return this.repo.findAll();
    }

    public void addPrietenie(long id1, long id2)
    {
       try{
           Prietenie prietenie = new Prietenie(min(id1, id2), max(id2, id1), LocalDate.now());
           prietenie.setId(new Tuple<Long, Long>(min(id1, id2), max(id2, id1)));
           prietenie.setFrom(id1);
           PrietenieValidator.validare_cu_repo(prietenie,repoPrietenie, repo);
           this.repoPrietenie.save(prietenie);
           EntityChangeEvent<Prietenie> event = new EntityChangeEvent<>(ChangeEventType.ADD, prietenie);
           notifyObservers(event);
       } catch (ValidationException e){
           throw new ValidationException(e.getMessage());
       }
    }

    public void acceptPrietenie(Prietenie prietenie)
    {
        if(this.repoPrietenie.findOne(prietenie.getId()).isEmpty())
            throw new ValidationException("Prietenie negasita");
        try{
            prietenie.setAccepted(true);
            Prietenie oldPrietenie = this.repoPrietenie.update(prietenie).get();
            EntityChangeEvent<Prietenie> event = new EntityChangeEvent<>(ChangeEventType.UPDATE, prietenie, oldPrietenie);
            notifyObservers(event);
        } catch (ValidationException e){
            throw new ValidationException(e.getMessage());
        }
    }

    public Iterable<Prietenie> get_requests(Long id1)
    {
        List<Prietenie> prietenii = new ArrayList<>();
        this.repoPrietenie.findAllRequests().forEach(prietenie -> {
            if(prietenie.getId1() == id1 || prietenie.getId2() == id1)
                prietenii.add(prietenie);
        });
        return prietenii;
    }

    public void deletePrietenie(long id1, long id2)
    {
        if(this.repoPrietenie.findOne(new Tuple<Long,Long>(min(id1, id2), max(id2, id1)) ).isEmpty())
            throw new ValidationException("Prietenie negasita");
        try{

           Prietenie prietenie = this.repoPrietenie.delete(new Tuple<Long, Long>(min(id1, id2), max(id1, id2))).get();
            EntityChangeEvent<Prietenie> event = new EntityChangeEvent<>(ChangeEventType.DELETE, prietenie);
            notifyObservers(event);
        } catch (ValidationException e){
            throw new ValidationException(e.getMessage());
        }
    }

    public Iterable<Prietenie> all_prietenii()
    {
        return this.repoPrietenie.findAll();
    }

    public void addMessage(String continut, Long from, Long to)
    {
        try{
            LocalDateTime date = LocalDateTime.now();
            Message message = new Message(continut, from, to, date);
            this.repoMessage.save(message);
            Optional<Message> mesaj = this.repoMessage.findOne(message.getId());
            EntityChangeEvent<Message> event = new EntityChangeEvent<>(ChangeEventType.ADD, mesaj.get());
            notifyObservers(event);
        } catch (ValidationException e) {
            throw new ValidationException(e.getMessage());
        }
    }

    public List<Message> getMessages(Long id1, Long id2){
        List<Message> messages = new ArrayList<>();
        this.repoMessage.findAll().forEach(message -> {
            if ((message.getFrom() == id1 && message.getTo() == id2) ||
                    (message.getFrom() == id2 && message.getTo() == id1)) {
                messages.add(message);
            }
        });
        return messages.stream()
                .sorted(Comparator.comparing(Message::getDate))
                .collect(Collectors.toList());
    }

    public void replyMessage(String continut, Long id_from, Long id_to, Long idMessage)
    {
        try{
            LocalDateTime date = LocalDateTime.now();
            Message reply = new Message(continut, id_from, id_to, date);
            reply.setId_reply(idMessage);
            Optional<Message> mesaj = this.repoMessage.save(reply);
            EntityChangeEvent<Message> event = new EntityChangeEvent<>(ChangeEventType.ADD, mesaj.get());
            notifyObservers(event);
        } catch (ValidationException e) {
            throw new ValidationException(e.getMessage());
        }
    }

    public Optional<Message> findOneMessage(Long id)
    {
        return this.repoMessage.findOne(id);
    }


}
