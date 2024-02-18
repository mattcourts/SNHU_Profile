package contactService;

import org.junit.Assert;
import org.junit.Test;

/*
 * Check conditions
 * Add, Delete,
 * Update First, Last, Number, Address
 * Checking for expected and unexpected ID's and confirmation of change.
 * */

public class ContactServiceTest {
    protected Contact testContact = new Contact("0123456789", "John", "Doe", "555123456", "123 Nowhere dr.");

    public ContactServiceTest() {
    }

    @Test
    public void testAdd() {


        ContactService cs = new ContactService();

        Assert.assertTrue(cs.Add(testContact)); //first add
        Assert.assertFalse(cs.Add(testContact)); //cannot add twice

    }

    @Test
    public void testDelete() {

        ContactService cs = new ContactService();
        cs.Add(testContact);

        Assert.assertTrue(cs.Delete("0123456789"));
        Assert.assertFalse(cs.Delete("0123456789")); //should not delete the same input twice
        Assert.assertFalse(cs.Delete("1111111111")); //does not exist
    }

    @Test
    public void testUpdateFirstName() {
        ContactService cs = new ContactService();
        cs.Add(testContact);

        Assert.assertTrue(cs.UpdateFirstName("0123456789", "Jane"));
        Assert.assertEquals("Jane", cs.SearchByID("0123456789").get_firstname()); //confirmation
        Assert.assertFalse(cs.UpdateFirstName("1111111111", "Jane"));
    }

    @Test
    public void testUpdateLastName() {
        ContactService cs = new ContactService();
        cs.Add(testContact);

        Assert.assertTrue(cs.UpdateLastName("0123456789", "Smith"));
        Assert.assertEquals("Smith", cs.SearchByID("0123456789").get_lastname()); //confirmation
        Assert.assertFalse(cs.UpdateLastName("1111111111", "Smith"));
    }

    @Test
    public void testUpdateNumber() {
        ContactService cs = new ContactService();
        cs.Add(testContact);

        Assert.assertTrue(cs.UpdateNumber("0123456789", "5555555555"));
        Assert.assertEquals("5555555555", cs.SearchByID("0123456789").get_number()); //confirmation
        Assert.assertFalse(cs.UpdateNumber("1111111111", "5555551234"));
    }

    @Test
    public void testUpdateAddress() {
        ContactService cs = new ContactService();
        cs.Add(testContact);

        Assert.assertTrue(cs.UpdateAddress("0123456789", "555 1st St."));
        Assert.assertEquals("555 1st St.", cs.SearchByID("0123456789").get_address()); //confirmation
        Assert.assertFalse(cs.UpdateAddress("1111111111", "123 2nd St."));
    }

}
