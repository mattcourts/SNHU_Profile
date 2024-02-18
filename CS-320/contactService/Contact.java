package contactService;

public class Contact {

    /* class private variables */
    private String _id;
    private String _firstname;
    private String _lastname;
    private String _number;
    private String _address;


    /* constructor */
    public Contact(String IDIn, String FirstNameIn, String LastNameIn, String PhoneIn, String AddressIn) {
        set_id(IDIn);
        set_firstname(FirstNameIn);
        set_lastname(LastNameIn);
        set_number(PhoneIn);
        set_address(AddressIn);
    }


    /* Setter functions */

    public String get_id() {
        return _id;
    }

    //can never be changed, set to private
    private void set_id(String _id) {

        /* Exit conditions*/
        if (_id == null) {
            throw new IllegalArgumentException();
        }

        if (_id.length() > 10) {
            throw new IllegalArgumentException();
        }

        /* set property */
        this._id = _id;
    }

    public String get_address() {
        return _address;
    }

    public void set_address(String _address) {

        /* Exit conditions*/
        if (_address == null) {
            throw new IllegalArgumentException();
        }

        if (_address.length() > 30) {
            throw new IllegalArgumentException();
        }

        /* set property */
        this._address = _address;
    }

    public String get_firstname() {
        return _firstname;
    }

    /* Getter functions */

    public void set_firstname(String _firstname) {

        /* Exit conditions*/
        if (_firstname == null) {
            throw new IllegalArgumentException();
        }

        if (_firstname.length() > 10) {
            throw new IllegalArgumentException();
        }

        /* set property */
        this._firstname = _firstname;
    }

    public String get_lastname() {
        return _lastname;
    }

    public void set_lastname(String _lastname) {

        /* Exit conditions*/
        if (_lastname == null) {
            throw new IllegalArgumentException();
        }

        if (_lastname.length() > 10) {
            throw new IllegalArgumentException();
        }

        /* set property */
        this._lastname = _lastname;
    }

    public String get_number() {
        return _number;
    }

    public void set_number(String _number) {

        /* Exit conditions*/
        if (_number == null) {
            throw new IllegalArgumentException();
        }

        if (_number.length() > 10) {
            throw new IllegalArgumentException();
        }

        /* set property */
        this._number = _number;
    }


}
