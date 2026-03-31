package ServiceBasedArch.bookservice;
import java.time.LocalDate;
import java.util.UUID;
import java.util.List;
import java.util.NoSuchElementException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
@RestController
@RequestMapping("api/books")
public class BookRestController{
    @Autowired
    private BookRepository bookRepository;
    @GetMapping(path = "")
    @ResponseStatus(code = HttpStatus.OK)
    public List<Book> findAllBooks(){return bookRepository.findAll();}
    @GetMapping(path = "/{bookGuid}")
    @ResponseStatus(code = HttpStatus.OK)
    public Book getBook(@PathVariable UUID bookGuid){return bookRepository.findById(bookGuid).orElseThrow(() -> new NoSuchElementException());}
    @GetMapping(path = "test")
    @ResponseStatus(code = HttpStatus.OK)
    public String test(){return "hello world";}
    @PostMapping(path = "")
    @ResponseStatus(code = HttpStatus.CREATED)
    public void CreateBook(@RequestBody Book book){
        book.setBookGuid(UUID.randomUUID());
        book.setCreatedDate(LocalDate.now());
        book.setPublishedDate((createRandomDate(1900, 2026)));
        bookRepository.save(book);
    }
    private static LocalDate createRandomDate(int startYear, int endYear){
        int day = createRandomIntBetween(1, 28);
        int month = createRandomIntBetween(1, 12);
        int year = createRandomIntBetween(startYear, endYear);
        return LocalDate.of(year, month, day);
    }
    private static int createRandomIntBetween(int start, int end){return start + (int) Math.round(Math.random() * (end - start));}
}
