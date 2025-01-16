package com.example.demo1.repository.database;

import com.example.demo1.domain.Message;
import com.example.demo1.repository.Repository;

import java.sql.*;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;

public class MessageDbRepository implements Repository<Long, Message> {
    private String url;
    private String username;
    private String password;

    public MessageDbRepository(String url, String username, String password)
    {
        this.url = url;
        this.username = username;
        this.password = password;
    }

    @Override
    public Optional<Message> findOne(Long aLong) {
        Message message = null;
        try(Connection connection = DriverManager.getConnection(url,username,password);
            PreparedStatement statement = connection.prepareStatement("SELECT * from messages WHERE id = ?");)
        {
            statement.setLong(1, aLong);
            ResultSet resultSet = statement.executeQuery();
            if(resultSet.next())
            {
                Long id = resultSet.getLong("id");
                String continut = resultSet.getString("continut");
                Long from = resultSet.getLong("id_from");
                Long to = resultSet.getLong("id_to");
                Timestamp date = resultSet.getTimestamp("date");
                Object id_reply = resultSet.getObject("id_reply");
                if(id_reply==null)
                    message = new Message(continut, from, to, date.toLocalDateTime());
                else
                {
                    message = new Message(continut, from, to, date.toLocalDateTime());
                    message.setId_reply((Long) id_reply);
                }
                message = new Message(continut, from, to, date.toLocalDateTime());
                message.setId(id);
            }
            resultSet.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return Optional.ofNullable(message);
    }

    @Override
    public Iterable<Message> findAll() {
        Set<Message> messages = new HashSet<>();
        try(Connection connection = DriverManager.getConnection(url,username,password);
            PreparedStatement statement = connection.prepareStatement("SELECT * from messages");
            ResultSet resultSet = statement.executeQuery())
        {
            while(resultSet.next())
            {
                Long id = resultSet.getLong("id");
                String continut = resultSet.getString("continut");
                Long from = resultSet.getLong("id_from");
                Long to = resultSet.getLong("id_to");
                Timestamp date = resultSet.getTimestamp("date");
                Long id_reply = resultSet.getLong("id_reply");
                Message message = new Message(continut, from, to, date.toLocalDateTime());
                message.setId(id);
                message.setId_reply(id_reply);
                messages.add(message);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return messages;
    }

    @Override
    public Optional<Message> save(Message entity) {
        if(entity==null)
            throw new IllegalArgumentException("entity must not be null");
        try(Connection connection = DriverManager.getConnection(url,username,password);
            PreparedStatement statement = connection.prepareStatement("INSERT INTO messages(continut, id_from, id_to, date, id_reply) VALUES(?,?,?,?,?) RETURNING id");)
        {
            statement.setString(1, entity.getContinut());
            statement.setLong(2, entity.getFrom());
            statement.setLong(3, entity.getTo());
            statement.setTimestamp(4, Timestamp.valueOf(entity.getDate()));
            if(entity.getId_reply()==null)
                statement.setNull(5, Types.BIGINT);
            else
                statement.setLong(5, entity.getId_reply());
            ResultSet resultSet = statement.executeQuery();
            if(resultSet.next())
            {
                Long id = resultSet.getLong("id");
                entity.setId(id);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return Optional.of(entity);
    }

    @Override
    public Optional<Message> delete(Long aLong) {
        try(Connection connection = DriverManager.getConnection(url,username,password);
            PreparedStatement statement = connection.prepareStatement("DELETE FROM messages WHERE id = ?");)
        {
            statement.setLong(1, aLong);
            statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return Optional.empty();
    }

    @Override
    public Optional<Message> update(Message entity) {
        if(entity==null)
            throw new IllegalArgumentException("entity must not be null");
        int rez=-1;
        try(Connection connection = DriverManager.getConnection(url,username,password);
            PreparedStatement statement = connection.prepareStatement("UPDATE messages SET continut = ?, from = ?, to = ?, date = ?, id_reply = ? WHERE id = ?");)
        {
            statement.setString(1, entity.getContinut());
            statement.setLong(2, entity.getFrom());
            statement.setLong(3, entity.getTo());
            statement.setTimestamp(4, Timestamp.valueOf(entity.getDate()));
            statement.setLong(5, entity.getId_reply());
            statement.setLong(6, entity.getId());
            rez = statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        if(rez>0)
            return Optional.empty();
        return Optional.of(entity);
    }
}
