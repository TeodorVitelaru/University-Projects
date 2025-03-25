package com.example.demo1.repository.file;

import com.example.demo1.domain.Entity;
import com.example.demo1.domain.Utilizator;
import com.example.demo1.domain.validators.Validator;
import com.example.demo1.repository.Repository;
import com.example.demo1.repository.memory.InMemoryRepository;

import java.io.*;
import java.nio.Buffer;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.Scanner;


public abstract class AbstractFileRepository<ID, E extends Entity<ID>> extends InMemoryRepository<ID, E>{
    private String filename;

    public AbstractFileRepository(Validator<E> validator, String fileName) {
        super(validator);
        this.filename=fileName;
        loadData();
    }

    private void loadData() {
        try {
            File file = new File(this.filename);
            if (!file.exists()) {
                System.out.println("File not found: " + this.filename);
                return;
            }
            if (!file.canRead()) {
                System.out.println("Cannot read file: " + this.filename);
                return;
            }
            var fileReader = new Scanner(new File(this.filename));
            while (fileReader.hasNextLine()) {
                String input = fileReader.nextLine();
                super.save(createEntity(input));
            }
            fileReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public abstract E createEntity(String line);
    public abstract String saveEntity(E entity);

    @Override
    public Optional<E> findOne(ID id) {
        return super.findOne(id);
    }

    @Override
    public Iterable<E> findAll() {
        return super.findAll();
    }

    @Override
    public Optional<E> save(E entity) {
        Optional<E> e = super.save(entity);
        if (e.isEmpty())
            writeToFile();
        return e;
    }

    private void writeToFile() {

        try  ( BufferedWriter writer = new BufferedWriter(new FileWriter(filename))){
            for (E entity: entities.values()) {
                String ent = saveEntity(entity);
                writer.write(ent);
                writer.newLine();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public Optional<E> delete(ID id) {
        Optional<E> deletedEntity = super.delete(id); // Use the inherited method from InMemoryRepository
        if (deletedEntity.isPresent()) {
            writeToFile(); // Update the file after deletion
        }
        return deletedEntity;
    }

    @Override
    public Optional<E> update(E entity) {
        Optional<E> updatedEntity = super.update(entity); // Use the inherited method from InMemoryRepository
        if (updatedEntity.isEmpty()) {
            writeToFile(); // Update the file after the update
        }
        return updatedEntity;
    }
}
