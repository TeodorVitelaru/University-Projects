package com.example.demo1.controller;

import com.example.demo1.domain.Message;
import com.example.demo1.domain.Utilizator;
import com.example.demo1.service.ServiceUtilizator;
import com.example.demo1.utils.events.ChangeEventType;
import com.example.demo1.utils.events.EntityChangeEvent;
import com.example.demo1.utils.observer.Observer;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.VBox;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Objects;
import java.util.Optional;

public class ChatController implements Observer<EntityChangeEvent> {

    private ServiceUtilizator service;
    private Utilizator loggedInUser;
    private Utilizator friend;
    private Message repliedMessage;

    @FXML
    private TextField messageField;
    @FXML
    private ScrollPane scrollPane;
    @FXML
    private VBox messageBox;
    @FXML
    private Label chatWithLabel;
    @FXML
    private Label repliedMessageLabel;

    public void setService(ServiceUtilizator service, Utilizator loggedInUser, Utilizator friend) {
        this.service = service;
        this.loggedInUser = loggedInUser;
        this.friend = friend;
        this.service.addObserver(this);
        loadMessages();
        setChatWithLabel();
        repliedMessageLabel.setVisible(false);
    }

    public void setChatWithLabel() {
        chatWithLabel.setText("Chat with " + friend.getFirstName() + " " + friend.getLastName());
    }

    private void loadMessages() {
        List<Message> messages = service.getMessages(loggedInUser.getId(), friend.getId());
        messages.forEach(message -> {
            Utilizator user = service.findOneUser(message.getFrom()).get();
            String messageText;
            if(message.getId_reply() != 0) {
                Optional<Message> r = service.findOneMessage(message.getId_reply());
                messageText = user.getFirstName() + " " + user.getLastName() + ":  " + message.getContinut() + " | (reply to: " + r.get().getContinut() + ")";
            } else{
                messageText = user.getFirstName()+ " " + user.getLastName() + ":  " + message.getContinut();
            }
            Label messageLabel = new Label(messageText);

            if(user.getId()==loggedInUser.getId())
                messageLabel.setStyle("-fx-background-color: #d0e7ff");
            else
            {
                messageLabel.setStyle("-fx-background-color: #d4ffd4");
            }
            messageLabel.setOnMouseClicked(event -> handleReply(message));
            messageBox.getChildren().add(messageLabel);
        });
        scrollPane.setContent(messageBox);
        scrollPane.setVvalue(1.0);
    }

    public void handleSendMessage() {
        String message = messageField.getText();
        if(repliedMessage != null) {
            service.replyMessage(message, loggedInUser.getId(), friend.getId(), repliedMessage.getId());
        } else {
            service.addMessage(message, loggedInUser.getId(), friend.getId());
        }

        messageField.clear();
        repliedMessageLabel.setVisible(false);
        repliedMessage = null;
    }

    public void handleReply(Message message) {
        if(message != null) {

            repliedMessageLabel.setText("Replying to: " + message.getContinut());
            repliedMessageLabel.setVisible(true);
            repliedMessage = message;
            repliedMessageLabel.setOnMouseClicked(event -> {
                repliedMessageLabel.setVisible(false);
                repliedMessageLabel.setText("");
                repliedMessage = null;
            });
        } else
        {
            repliedMessageLabel.setVisible(false);
        }
    }

    @Override
    public void update(EntityChangeEvent entityChangeEvent) {
        if(entityChangeEvent.getData() instanceof Message)
        {
            Message message = (Message) entityChangeEvent.getData();
            if(entityChangeEvent.getType().equals(ChangeEventType.ADD))
            {
                String messageText;
                Label messageLabel = new Label();
                Utilizator userFrom = service.findOneUser(message.getFrom()).get();

                if(message.getId_reply() != null) {
                    Message repliedMessage = service.findOneMessage(message.getId_reply()).get();
                    messageText = userFrom.getFirstName() + " " + userFrom.getLastName() + ":  " + message.getContinut() + " | (reply to: " + repliedMessage.getContinut() + ")";
                } else {
                    messageText = userFrom.getFirstName() + " " + userFrom.getLastName() + ":  " + message.getContinut();
                }

                if(Objects.equals(userFrom.getId(), loggedInUser.getId()))
                {
                    messageLabel.setStyle("-fx-background-color: #d0e7ff");
                    messageLabel.setAlignment(Pos.CENTER_RIGHT);
                }
                else
                {
                    messageLabel.setStyle("-fx-background-color: #d4ffd4");
                    messageLabel.setAlignment(Pos.CENTER_LEFT);
                }

                messageLabel.setText(messageText);
                messageLabel.setOnMouseClicked(event -> handleReply(message));
                messageBox.getChildren().add(messageLabel);
                scrollPane.setContent(messageBox);
                messageBox.heightProperty().addListener((observable, oldValue, newValue) -> scrollPane.setVvalue(1.0));
            }

        }
    }
}
