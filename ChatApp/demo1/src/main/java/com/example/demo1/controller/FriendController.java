package com.example.demo1.controller;

import com.example.demo1.domain.Prietenie;
import com.example.demo1.domain.Utilizator;
import com.example.demo1.service.ServiceUtilizator;
import com.example.demo1.utils.events.ChangeEventType;
import com.example.demo1.utils.events.EntityChangeEvent;
import com.example.demo1.utils.observer.Observer;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;

import java.io.PrintStream;
import java.util.Objects;
import java.util.Optional;

public class FriendController implements Observer<EntityChangeEvent> {
    private ServiceUtilizator service;
    private Utilizator loggedInUser;
    private ObservableList<Prietenie> modelRequests = FXCollections.observableArrayList();


    @FXML
    TableView<Prietenie> tblRequests;
    @FXML
    TableColumn<Prietenie, String> tableColumnDateRequests;
    @FXML
    TableColumn<Prietenie, Boolean> tabelColumnStatusRequests;
    @FXML
    TableColumn<Prietenie, String> tabelColumnFromRequests;


    public void setService(ServiceUtilizator service, Utilizator loggedInUser) {
        this.service = service;
        this.loggedInUser = loggedInUser;
        this.service.addObserver(this);
        initModel();
    }

    @FXML
    public void initialize()
    {
        tableColumnDateRequests.setCellValueFactory(new PropertyValueFactory<>("date"));
        tabelColumnStatusRequests.setCellValueFactory(new PropertyValueFactory<>("accepted"));
        tabelColumnFromRequests.setCellValueFactory(cellData ->
        {
            Prietenie prietenie = cellData.getValue();
            Optional<Utilizator> user = service.findOneUser(prietenie.getFrom());
            return new SimpleStringProperty(user.get().getFirstName() + " " + user.get().getLastName());
        });
        tblRequests.setItems(modelRequests);
    }

    private void initModel()
    {
        for(Prietenie prietenie : service.get_requests(loggedInUser.getId()))
        {
            if(!Objects.equals(prietenie.getFrom(), loggedInUser.getId()) && (Objects.equals(prietenie.getId1(), loggedInUser.getId()) || Objects.equals(prietenie.getId2(), loggedInUser.getId())))
            {
                modelRequests.add(prietenie);
            }
        }
        this.initialize();
    }

    public void handleAcceptFrined() {
        Prietenie prietenie = tblRequests.getSelectionModel().getSelectedItem();
        service.acceptPrietenie(prietenie);
    }

    public void handleDeclineFriend() {
        Prietenie prietenie = tblRequests.getSelectionModel().getSelectedItem();
        if(prietenie != null)
            service.deletePrietenie(prietenie.getId1(), prietenie.getId2());
        else
            System.out.println("Selecteaza o cerere");
    }

    @Override
    public void update(EntityChangeEvent utilizatorEntityChangeEvent) {
        if(utilizatorEntityChangeEvent.getData() instanceof Prietenie)
        {
            if (utilizatorEntityChangeEvent.getType().equals(ChangeEventType.UPDATE))
            {
                Prietenie prietenie = (Prietenie) utilizatorEntityChangeEvent.getOldData();
                modelRequests.remove(prietenie);
            }
            if (utilizatorEntityChangeEvent.getType().equals(ChangeEventType.DELETE))
            {

                Prietenie prietenie = (Prietenie) utilizatorEntityChangeEvent.getData();
                modelRequests.remove(prietenie);
            }
            if(utilizatorEntityChangeEvent.getType().equals(ChangeEventType.ADD))
            {
                Prietenie prietenie = (Prietenie) utilizatorEntityChangeEvent.getData();
                modelRequests.add(prietenie);
            }
        }
    }
}
