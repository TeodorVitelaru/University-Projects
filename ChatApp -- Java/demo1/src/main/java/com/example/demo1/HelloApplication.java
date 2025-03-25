package com.example.demo1;

import com.example.demo1.controller.LoginController;
import com.example.demo1.controller.UtilizatorController;
import com.example.demo1.domain.Message;
import com.example.demo1.domain.validators.PrietenieValidator;
import com.example.demo1.domain.validators.UtilizatorValidator;
import com.example.demo1.repository.database.MessageDbRepository;
import com.example.demo1.repository.database.PrietenieDbRepository;
import com.example.demo1.repository.database.UtilizatorDbRepository;
import com.example.demo1.service.ServiceUtilizator;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import org.controlsfx.control.spreadsheet.Grid;


import java.io.IOException;

public class HelloApplication extends Application {
    ServiceUtilizator service;

    @Override
    public void start(Stage primaryStage) throws IOException {
        String url = "jdbc:postgresql://localhost:5432/labMAP";
        String username = "postgres";
        String password = "postgres";
        PrietenieDbRepository prietenieDbRepository = new PrietenieDbRepository(url, username, password, new PrietenieValidator());
        UtilizatorDbRepository utilizatorDbRepository = new UtilizatorDbRepository(url, username, password, new UtilizatorValidator());
        MessageDbRepository messageDbRepository = new MessageDbRepository(url, username, password);
        this.service = new ServiceUtilizator(utilizatorDbRepository, prietenieDbRepository, messageDbRepository);
        initView(primaryStage);
        primaryStage.setWidth(800);
        primaryStage.show();
    }

    private void initView(Stage primaryStage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("LoginView.fxml"));
        
        GridPane loginLayout = fxmlLoader.load();
        primaryStage.setScene(new Scene(loginLayout));
        
        LoginController utilizatorController = fxmlLoader.getController();
        utilizatorController.setService(this.service);
    }

    public static void main(String[] args) {
        launch();
    }
}