package ServiceBasedArch.bookservice;
import java.time.LocalDate;
import java.util.UUID;
import org.springframework.data.annotation.*;
//import jakarta.persistence.*;
//import com.fasterxml.jackson.annotation.*;
public class Book{
    @Id
    private UUID bookGuid;
    // @JsonProperty("Title")
    private String title;
    // @JsonProperty("Author")
    private String author;
    // @JsonProperty("Description")
    private String description;
    private Double price;
    private LocalDate publishedDate, createdDate;
    public UUID getBookGuid(){return bookGuid;}
    public void setBookGuid(UUID bookGuid){this.bookGuid = bookGuid;}
    public String getTitle(){return title;}
    public void setTitle(String title){this.title = title;}
    public String getAuthor(){return author;}
    public void setAuthor(String author){this.author = author;}
    public String getDescription(){return description;}
    public void setDescription(String description){this.description = description;}
    public Double getPrice(){return price;}
    public void setPrice(Double price){this.price = price;}
    public LocalDate getPublishedDate(){return publishedDate;}
    public void setPublishedDate(LocalDate publishedDate){this.publishedDate = publishedDate;}
    public LocalDate getCreatedDate(){return createdDate;}
    public void setCreatedDate(LocalDate createdDate){this.createdDate = createdDate;}  
}