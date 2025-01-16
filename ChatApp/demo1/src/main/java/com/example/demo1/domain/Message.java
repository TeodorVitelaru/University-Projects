package com.example.demo1.domain;

import java.time.LocalDate;
import java.time.LocalDateTime;

public class Message extends Entity<Long> {
    private Long id;
    private String continut;
    private Long from;
    private Long to;
    private LocalDateTime date;
    private Long id_reply;

    public Message(String continut, Long from, Long to, LocalDateTime date) {
        this.continut = continut;
        this.from = from;
        this.to = to;
        this.date = date;
        this.id_reply = null;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Long getId() {
        return this.id;
    }

    public void setId_reply(Long id_reply) {
        this.id_reply = id_reply;
    }

    public Long getId_reply() {
        return this.id_reply;
    }

    public String getContinut() {
        return this.continut;
    }

    public Long getFrom() {
        return this.from;
    }

    public Long getTo() {
        return this.to;
    }

    public LocalDateTime getDate() {
        return this.date;
    }

    @Override
    public String toString() {
        return "Message{" +
                "continut='" + continut + '\'' +
                ", from=" + from +
                ", to=" + to +
                ", date=" + date +
                ", id_reply=" + id_reply +
                '}';
    }
}
