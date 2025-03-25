package com.example.demo1.repository.database;

import com.example.demo1.domain.Page;
import com.example.demo1.domain.Pageable;
import com.example.demo1.domain.Utilizator;
import com.example.demo1.domain.validators.Validator;
import com.example.demo1.repository.Repository;

import java.sql.*;
import java.util.*;

public class UtilizatorDbRepository implements PagingRepository<Long, Utilizator> {

    private String url;
    private String username;
    private String password;
    private Validator<Utilizator> validator;

    public UtilizatorDbRepository(String url, String username, String password, Validator<Utilizator> validator)
    {
        this.url = url;
        this.username = username;
        this.password = password;
        this.validator = validator;
    }

    private List<Utilizator> findAllOnPage(Connection connection, Pageable pageable) throws SQLException {
        List<Utilizator> usersOnPage = new ArrayList<>();
        String sql = "SELECT * FROM users LIMIT ? OFFSET ?";


        try(PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setInt(1, pageable.getPageSize());
            statement.setInt(2, pageable.getPageSize() * pageable.getPageNumber());
            ResultSet resultSet = statement.executeQuery();

            while(resultSet.next()) {
                Long id = resultSet.getLong("id");
                String firstName = resultSet.getString("first_name");
                String lastName = resultSet.getString("last_name");

                Utilizator utilizator = new Utilizator(firstName, lastName);
                utilizator.setId(id);
                usersOnPage.add(utilizator);
            }
        }
        return usersOnPage;
    }

    @Override
    public Page<Utilizator> findAllOnPage(Pageable pageable) {
        try (Connection connection = DriverManager.getConnection(url, username, password)) {
           int totalNumberOfUsers = count(connection);
           List<Utilizator> usersOnPage;
           if(totalNumberOfUsers > 0)
                {
                usersOnPage = findAllOnPage(connection, pageable);
              }
              else
              {
                usersOnPage = new ArrayList<>();
                     }
              return new Page<>(usersOnPage, totalNumberOfUsers);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private int count(Connection connection) throws SQLException {
        String sql = "SELECT COUNT(*) FROM users";
        try(PreparedStatement statement = connection.prepareStatement(sql);
            ResultSet resultSet = statement.executeQuery()) {
            if(resultSet.next()) {
                return resultSet.getInt(1);
            }
        }
        return 0;
    }


    @Override
    public Optional<Utilizator> findOne(Long aLong) {
        Utilizator utilizator = null;
        try (Connection connection = DriverManager.getConnection(url, username, password);
             PreparedStatement statement = connection.prepareStatement("SELECT * from users WHERE id = ?");
        ) {
            statement.setLong(1, aLong);
            ResultSet resultSet = statement.executeQuery();
            if (resultSet.next()) {
                Long id = resultSet.getLong("id");
                String firstName = resultSet.getString("first_name");
                String lastName = resultSet.getString("last_name");

                utilizator = new Utilizator(firstName, lastName);
                utilizator.setId(id);
            }
            resultSet.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        return Optional.ofNullable(utilizator);
    }


    @Override
    public Iterable<Utilizator> findAll() {
        Set<Utilizator> users = new HashSet<>();
        try(Connection connection = DriverManager.getConnection(url, username, password);
            PreparedStatement statement = connection.prepareStatement("SELECT * from users");
            ResultSet resultSet = statement.executeQuery()) {

            while(resultSet.next())
            {
                Long id = resultSet.getLong("id");
                String firstName = resultSet.getString("first_name");
                String lastName = resultSet.getString("last_name");

                Utilizator utilizator = new Utilizator(firstName, lastName);
                utilizator.setId(id);
                users.add(utilizator);
            }
            return users;
           } catch (SQLException e) {
            e.printStackTrace();
        }
        return users;
    }

    @Override
    public Optional<Utilizator> save(Utilizator entity) {
        if(entity==null)
            throw new IllegalArgumentException("ENTITY CANNOT BE NULL");
        validator.validate(entity);
        try (Connection connection = DriverManager.getConnection(url, username, password);
             PreparedStatement statement = connection.prepareStatement("INSERT INTO users(first_name, last_name) VALUES (?, ?) RETURNING id");
             PreparedStatement statement1 = connection.prepareStatement("INSERT INTO infos(username, password) VALUES (?, ?)")) {
            statement.setString(1, entity.getFirstName());
            statement.setString(2, entity.getLastName());
            ResultSet resultSet = statement.executeQuery();
            if(resultSet.next())
            {
                Long id = resultSet.getLong("id");
                entity.setId(id);
            }

            statement1.setString(1, entity.getFirstName());
            statement1.setString(2, entity.getFirstName());
            statement1.executeUpdate();

        } catch (SQLException e) {
            e.printStackTrace();
        }

        return Optional.of(entity);

    }

    @Override
    public Optional<Utilizator> delete(Long aLong) {
        try(Connection connection = DriverManager.getConnection(url, username, password);
            PreparedStatement statement = connection.prepareStatement("DELETE FROM users WHERE id = ?");
            PreparedStatement statement1 = connection.prepareStatement("DELETE FROM infos where username = ?")) {
            statement.setLong(1, aLong);
            Optional<Utilizator> utilizator = findOne(aLong);
            statement.executeUpdate();
            return utilizator;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return Optional.empty();
    }

    @Override
    public Optional<Utilizator> update(Utilizator entity) {
        int rez = -1;
        try(Connection connection = DriverManager.getConnection(url, username, password);
            PreparedStatement statement = connection.prepareStatement("UPDATE users SET first_name = ?, last_name = ? WHERE id = ?")) {
            statement.setString(1, entity.getFirstName());
            statement.setString(2, entity.getLastName());
            statement.setLong(3, entity.getId());
            rez = statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        if(entity==null)
            throw new IllegalArgumentException("ENTITY CANNOT BE NULL");
        validator.validate(entity);
        if(rez > 0)
            return null;
        return Optional.of(entity);

    }
}
