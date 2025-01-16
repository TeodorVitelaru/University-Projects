package com.example.demo1.controller;

import com.example.demo1.domain.Utilizator;
import com.example.demo1.service.ServiceUtilizator;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import jdk.jshell.execution.Util;

import javax.xml.transform.Result;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.Optional;

public class LoginController {
    ServiceUtilizator service;

    @FXML
    private TextField usernameField;

    @FXML
    private TextField passwordField;
    @FXML
    private Label label1;
    @FXML
    private Label label2;
    @FXML
    private Button logInButton;
    @FXML
    private Button signUpButton;
    @FXML
    private Button backToButton;

    public void setService(ServiceUtilizator service) {

        this.service = service;
        label1.setText("Username:");
        label2.setText("Password:");
        backToButton.setVisible(false);
        logInButton.setVisible(true);
        signUpButton.setVisible(true);
    }


    public void handleLogin() {
        String username = usernameField.getText();
        String password = passwordField.getText();
        Optional<Utilizator> utilizator = authenticate(username, password);
        if (utilizator.isPresent()) {
            openMainWindow(utilizator.get());
        } else {
            System.out.println("Login failed");
        }
        usernameField.clear();
        passwordField.clear();
    }

    private Optional<Utilizator> authenticate(String username, String password) {
        String url = "jdbc:postgresql://localhost:5432/labMAP";
        String dbUsername = "postgres";
        String dbPassword = "postgres";
        String query = "SELECT * FROM infos WHERE username = ? AND password = ?";
        try (Connection connection = DriverManager.getConnection(url, dbUsername, dbPassword);
             PreparedStatement statement = connection.prepareStatement(query)) {
            statement.setString(1, username);
            statement.setString(2, password);

            ResultSet result = statement.executeQuery();
            if(result.next())
            {
                Long id = result.getLong("id");
                return service.findOneUser(id);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return Optional.empty();
    }

    private void openMainWindow(Utilizator utilizator)
    {
        try{
            FXMLLoader fxmlLoader2 = new FXMLLoader(getClass().getResource("/com/example/demo1/UtilizatorView.fxml"));
            AnchorPane userLayout = fxmlLoader2.load();
            UtilizatorController utilizatorController = fxmlLoader2.getController();
            utilizatorController.setService(service, utilizator);
            Stage stage = new Stage();
            stage.setTitle(utilizator.getFirstName() + " " + utilizator.getLastName());
            stage.setScene(new Scene(userLayout));
            stage.setWidth(800);
            stage.setHeight(600);
            stage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleSignUp(ActionEvent actionEvent) {
        label1.setText("First Name:");
        label2.setText("Last Name:");
        logInButton.setVisible(false);
        signUpButton.setVisible(false);
        backToButton.setVisible(true);
    }

    public void handleAddUser(ActionEvent actionEvent) {
        try {
            String firstName = usernameField.getText();
            String lastName = passwordField.getText();

            if (firstName.isEmpty() || lastName.isEmpty()) {
                System.out.println("First name and last name must not be empty");
                return;
            }

            service.addUtilizator(firstName, lastName);

            label1.setText("Username:");
            label2.setText("Password:");
            logInButton.setVisible(true);
            signUpButton.setVisible(true);
            backToButton.setVisible(false);

            usernameField.clear();
            passwordField.clear();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
