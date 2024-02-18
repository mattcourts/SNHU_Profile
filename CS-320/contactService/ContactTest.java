package contactService;

import org.junit.Test;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertThrows;

public class ContactTest {

    protected Contact testContact;

    public ContactTest() {
        testContact = new Contact("0123456789", "John", "Doe", "555123456", "123 Nowhere dr.");
    }


    @Test
    public void testID() throws NoSuchMethodException {

        //check private
        Method editIDStatus = Contact.class.getDeclaredMethod("set_id", String.class);
        assertEquals(Modifier.PRIVATE, editIDStatus.getModifiers());

        //long
        Exception thrownLong = assertThrows(Exception.class, () -> new Contact("01234567890", "John", "Doe", "555123456", "123 Nowhere dr."));
        assertEquals(IllegalArgumentException.class, thrownLong.getClass());

        //null
        Exception thrownNull = assertThrows(Exception.class, () -> new Contact(null, "John", "Doe", "555123456", "123 Nowhere dr."));
        assertEquals(IllegalArgumentException.class, thrownNull.getClass());

    }

    @Test
    public void testFirstName() {

        String shortVal = "jan";
        String exactLen = "Jan-Kowski";
        String longVal = "John J. Doe jr.";


        Exception thrownLong = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_firstname(longVal));
        assertEquals(IllegalArgumentException.class, thrownLong.getClass());

        //short
        this.testContact.set_firstname(shortVal);
        assertEquals(shortVal, this.testContact.get_firstname());

        //exact
        this.testContact.set_firstname(exactLen);
        assertEquals(exactLen, this.testContact.get_firstname());

        //null
        Exception thrownNull = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_firstname(null));
        assertEquals(IllegalArgumentException.class, thrownNull.getClass());


    }

    @Test
    public void testLastName() {

        String shortVal = "Smith";
        String exactLen = "Van Rossum";
        String longVal = "John J. Doe jr.";


        Exception thrownLong = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_lastname(longVal));
        assertEquals(IllegalArgumentException.class, thrownLong.getClass());

        //short
        this.testContact.set_lastname(shortVal);
        assertEquals(shortVal, this.testContact.get_lastname());

        //exact
        this.testContact.set_lastname(exactLen);
        assertEquals(exactLen, this.testContact.get_lastname());

        //null
        Exception thrownNull = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_lastname(null));
        assertEquals(IllegalArgumentException.class, thrownNull.getClass());

    }

    @Test
    public void testNumber() {

        String shortVal = "555";
        String exactLen = "5555555555";
        String longVal = "(555)-555-5555";


        Exception thrownLong = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_number(longVal));
        assertEquals(IllegalArgumentException.class, thrownLong.getClass());

        //short
        this.testContact.set_number(shortVal);
        assertEquals(shortVal, this.testContact.get_number());

        //exact
        this.testContact.set_number(exactLen);
        assertEquals(exactLen, this.testContact.get_number());

        //null
        Exception thrownNull = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_number(null));
        assertEquals(IllegalArgumentException.class, thrownNull.getClass());

    }

    @Test
    public void testAddress() {

        String shortVal = "123 1st";
        String exactLen = "123 2nd st";
        String longVal = "123 First Street Cincinnati Missouri";


        Exception thrownLong = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_address(longVal));
        assertEquals(IllegalArgumentException.class, thrownLong.getClass());

        //short
        this.testContact.set_address(shortVal);
        assertEquals(shortVal, this.testContact.get_address());

        //exact
        this.testContact.set_address(exactLen);
        assertEquals(exactLen, this.testContact.get_address());

        //null
        Exception thrownNull = assertThrows(IllegalArgumentException.class, () -> this.testContact.set_address(null));
        assertEquals(IllegalArgumentException.class, thrownNull.getClass());

    }

}
