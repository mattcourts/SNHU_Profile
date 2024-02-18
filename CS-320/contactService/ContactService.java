package contactService;

import java.util.ArrayList;
import java.util.List;

public class ContactService {

    private final List<Contact> ContactList;

    public ContactService() {
        this.ContactList = new ArrayList<>();
    }

    public Contact SearchByID(String ID) {

        return ContactList.stream()
                .filter(contact -> ID.equals(contact.get_id()))
                .findFirst() // ID is unique, can only be one instance if any.
                .orElse(null);
    }

    public boolean Add(Contact newContact) {
        if (!this.ContactList.contains(newContact)) {
            return this.ContactList.add(newContact);
        }

        return false;

    }

    public Boolean Delete(String ID) {
        Contact toRemove = this.SearchByID(ID);

        if (toRemove == null) {
            return false;
        }

        return this.ContactList.remove(toRemove);
    }

    public Boolean UpdateFirstName(String ID, String FirstName) {

        Contact nameToUpdate = this.SearchByID(ID);

        if (nameToUpdate == null) {
            return false;
        }

        nameToUpdate.set_firstname(FirstName);

        return true;
    }

    public Boolean UpdateLastName(String ID, String LastName) {

        Contact nameToUpdate = this.SearchByID(ID);

        if (nameToUpdate == null) {
            return false;
        }

        nameToUpdate.set_lastname(LastName);

        return true;
    }

    public Boolean UpdateNumber(String ID, String Number) {

        Contact nameToUpdate = this.SearchByID(ID);

        if (nameToUpdate == null) {
            return false;
        }

        nameToUpdate.set_number(Number);

        return true;
    }

    public Boolean UpdateAddress(String ID, String Address) {

        Contact nameToUpdate = this.SearchByID(ID);

        if (nameToUpdate == null) {
            return false;
        }

        nameToUpdate.set_address(Address);

        return true;
    }

}
