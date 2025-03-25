package com.example.demo1.controller;

import com.example.demo1.domain.Page;
import com.example.demo1.domain.Pageable;
import com.example.demo1.domain.Prietenie;
import com.example.demo1.domain.Utilizator;
import com.example.demo1.service.ServiceUtilizator;
import com.example.demo1.utils.events.ChangeEventType;
import com.example.demo1.utils.events.EntityChangeEvent;
import com.example.demo1.utils.observer.Observer;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;


import java.io.IOException;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

public class UtilizatorController implements Observer<EntityChangeEvent> {
    ServiceUtilizator service;
    private ObservableList<Utilizator> model = FXCollections.observableArrayList();
    private ObservableList<Prietenie> modelPrieteni = FXCollections.observableArrayList();
    private ObservableList<Utilizator> filteredModel = FXCollections.observableArrayList();
    private Utilizator loggedInUser;
    private int currentPage = 0;
    private int PAGE_SIZE = 3;


    //pentru tabel cu useri
    @FXML
    private TableView<Utilizator> tableView;
    @FXML
    private TableColumn<Utilizator, String> tableColumnId;
    @FXML
    private TableColumn<Utilizator, String> tableColumnFirstName;
    @FXML
    private TableColumn<Utilizator, String> tableColumnLastName;
    @FXML
    private TextField searchBar;
    @FXML
    private TextField usersPerPageField;

    //pentru tabel cu prieteni
    @FXML
    private TableView<Prietenie> friendsTable;
    @FXML
    private TableColumn<Prietenie, String> friendNameColumn;
    @FXML
    private TableColumn<Prietenie, String> dateColumn;

    @FXML
    private Button friendRequestsButton;

    @FXML
    private Label labelPage;

    //butoanele din paginare
    @FXML
    private Button buttonPrev;
    @FXML
    private Button buttonNext;


    private void updateFriendRequestsButton() {
        AtomicInteger count = new AtomicInteger();
        service.get_requests(loggedInUser.getId()).forEach(prietenie -> {
                if(!Objects.equals(prietenie.getFrom(), loggedInUser.getId())) {
                    count.getAndIncrement();
                    System.out.println(prietenie);
                }
            });
        friendRequestsButton.setText("Friend Requests (" + count + ")");
    }

    public void setService(ServiceUtilizator service, Utilizator loggedInUser) {
        this.service = service;
        this.loggedInUser = loggedInUser;
        service.addObserver(this);
        initModel();
        initModelPrieteni();
        updateFriendRequestsButton();
        userNumberObserver();

    }

    public void userNumberObserver()
    {
        usersPerPageField.textProperty().addListener((observable, oldValue, newValue) -> {
            try {
                PAGE_SIZE = Integer.parseInt(newValue);
                if (PAGE_SIZE <= 0)
                    PAGE_SIZE = 3;
                currentPage = 0;
                buttonNext.setDisable(currentPage == service.getUsersOnPage(new Pageable(PAGE_SIZE, currentPage)).getTotalElements() / PAGE_SIZE);
                loadPage(currentPage);
            } catch (NumberFormatException e) {
                System.out.println("Introduceti un numar valid");
            }
        });
    }


    @FXML
    public void initialize() {
        tableColumnId.setCellValueFactory(new PropertyValueFactory<>("id"));
        tableColumnFirstName.setCellValueFactory(new PropertyValueFactory<>("firstName"));
        tableColumnLastName.setCellValueFactory(new PropertyValueFactory<>("lastName"));
        tableView.setItems(model);

    }

    private void initModel() {
        loadPage(currentPage);
        initialize();
    }


    private void loadPage(int page)
    {
        Pageable pageable = new Pageable(PAGE_SIZE, page);
        Page<Utilizator> usersOnPage = service.getUsersOnPage(pageable);
        buttonPrev.setDisable(page == 0);
        model.clear();
        model.setAll(usersOnPage.getContent());
        labelPage.setText("Page " + (page + 1) + " of " + (usersOnPage.getTotalElements() / PAGE_SIZE + 1));
    }

    private void initModelPrieteni() {
        modelPrieteni.clear();
        for(Prietenie prietenie : service.all_prietenii()) {
            if(prietenie.getId1() == loggedInUser.getId() || prietenie.getId2() == loggedInUser.getId())
                modelPrieteni.add(prietenie);
        }
        initializePrieteni();
    }

    public void handleDeleteFriend() {
        Prietenie prietenie = friendsTable.getSelectionModel().getSelectedItem();
        if (prietenie != null) {
            service.deletePrietenie(prietenie.getId1(), prietenie.getId2());
        }
    }

    public void handleAddFriend()
    {
        try{
            Utilizator user = tableView.getSelectionModel().getSelectedItem();
            if (user != null) {
                service.addPrietenie(loggedInUser.getId(), user.getId());
                System.out.println("Prietenie adaugata");
                initModelPrieteni();
            } else {
                System.out.println("Selecteaza un utilizator");
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }


    public void initializePrieteni()
    {
        friendNameColumn.setCellValueFactory(cellData ->{
            Prietenie prietenie = cellData.getValue();
            Optional<Utilizator> user;
            if (Objects.equals(prietenie.getId1(), loggedInUser.getId()))
                user = service.findOneUser(prietenie.getId2());
            else
                user = service.findOneUser(prietenie.getId1());
            return new SimpleStringProperty(user.get().getFirstName() + " " + user.get().getLastName());
        });
        dateColumn.setCellValueFactory(new PropertyValueFactory<>("date"));

        friendsTable.setItems(modelPrieteni);
    }

    public void handleViewFriendRequests()
    {
        try{
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/com/example/demo1/FriendRequestsView.fxml"));
            AnchorPane root = loader.load();
            FriendController controller = loader.getController();
            controller.setService(service, loggedInUser);
            Stage stage = new Stage();
            stage.setScene(new Scene(root));
            stage.setWidth(600);
            stage.show();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleSearch(ActionEvent actionEvent) {
        model.clear();
        if(this.searchBar.getText().isEmpty())
            initModel();
        List<Utilizator> gasiti = new ArrayList<>();
        for(Utilizator u : service.all_utilizators())
        {
            if(u.getFirstName().toLowerCase().startsWith(searchBar.getText().trim().toLowerCase()))
            {
                gasiti.add(u);
            }
        }
        model.setAll(gasiti);
    }

    public void handleRefresh(ActionEvent actionEvent) {
        this.initModel();
        this.initialize();
    }

    public void handleDeleteUser(ActionEvent actionEvent) {
        if(loggedInUser.getFirstName().equals("admin")) {
            Utilizator user = tableView.getSelectionModel().getSelectedItem();
            if (user != null) {
                service.deleteUtilizator(user.getId());
            }
        }
        else {
            System.out.println("Nu aveti permisiunea necesara");
        }
    }


    public void handleChat(ActionEvent actionEvent) {
        try{
            Prietenie prietenie = friendsTable.getSelectionModel().getSelectedItem();
            Utilizator friend;
            if(prietenie.getId1() == loggedInUser.getId())
                friend = service.findOneUser(prietenie.getId2()).get();
            else
                friend = service.findOneUser(prietenie.getId1()).get();

            if(friend == null) {
                System.out.println("Selecteaza un utilizator");
                return;
            }

            FXMLLoader loader = new FXMLLoader(getClass().getResource("/com/example/demo1/ChatView.fxml"));
            AnchorPane root = loader.load();
            ChatController controller = loader.getController();
            controller.setService(service, loggedInUser, friend);
            Stage stage = new Stage();
            stage.setScene(new Scene(root));
            stage.setWidth(600);
            stage.show();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }


    @Override
    public void update(EntityChangeEvent utilizatorEntityChangeEvent) {
        if(utilizatorEntityChangeEvent.getType().equals(ChangeEventType.ADD))
        {
            if (utilizatorEntityChangeEvent.getData() instanceof Utilizator) {
                Utilizator utilizator = (Utilizator) utilizatorEntityChangeEvent.getData();
                model.add(utilizator);
            }
            if(utilizatorEntityChangeEvent.getData() instanceof Prietenie)
            {
                updateFriendRequestsButton();
            }
        }
        if(utilizatorEntityChangeEvent.getType().equals(ChangeEventType.DELETE))
        {
            if (utilizatorEntityChangeEvent.getData() instanceof Utilizator) {
                Utilizator utilizator = (Utilizator) utilizatorEntityChangeEvent.getData();
                model.remove(utilizator);
            }
            if(utilizatorEntityChangeEvent.getData() instanceof Prietenie)
            {
                Prietenie prietenie = (Prietenie) utilizatorEntityChangeEvent.getData();
                modelPrieteni.remove(prietenie);
                updateFriendRequestsButton();
            }
        }
        if(utilizatorEntityChangeEvent.getType().equals(ChangeEventType.UPDATE))
        {
            if (utilizatorEntityChangeEvent.getData() instanceof Utilizator) {
                Utilizator utilizator = (Utilizator) utilizatorEntityChangeEvent.getOldData();
                Utilizator newUtilizator = (Utilizator) utilizatorEntityChangeEvent.getData();
                model.set(model.indexOf(utilizator), newUtilizator);
            }
            if (utilizatorEntityChangeEvent.getData() instanceof Prietenie) {
                Prietenie prietenie = (Prietenie) utilizatorEntityChangeEvent.getOldData();
                Prietenie newPrietenie = (Prietenie) utilizatorEntityChangeEvent.getData();
                modelPrieteni.add(newPrietenie);
                updateFriendRequestsButton();
            }
        }
    }

    public void handlePrevious(ActionEvent actionEvent) {
        if(currentPage > 0)
        {
            currentPage--;
            buttonPrev.setDisable(currentPage == 0);
            buttonNext.setDisable(false);
            loadPage(currentPage);
        }

    }

    public void handleNext(ActionEvent actionEvent) {
        if(currentPage < service.getUsersOnPage(new Pageable(PAGE_SIZE, currentPage)).getTotalElements() / PAGE_SIZE)
        {
            currentPage++;
            buttonPrev.setDisable(false);
            buttonNext.setDisable(currentPage == service.getUsersOnPage(new Pageable(PAGE_SIZE, currentPage)).getTotalElements() / PAGE_SIZE);
            loadPage(currentPage);
        }
    }

}
