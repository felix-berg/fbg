#include <fbg.hpp>
#include <fbtt.hpp>

void float_vector_test();
void alpha_composite_test();
void frame_test();
void shape_tests();

using namespace fbg;
using namespace fbtt;

int main() 
{
   float_vector_test();
   alpha_composite_test();
   frame_test();
   shape_tests();

   return 0;
}

void float_vector_test()
{
   MultiTest<V2d<float>> newVector { "Test of V2d vector class" };

   newVector.add_constructor(
      "Default constructor", [](auto & vec) {
         vec = new V2d<float>;
      }
   );

   newVector.add_constructor(
      "Constructor {0, 0}", [](auto & vec) {
         vec = new V2d<float> { 0, 0 };
      }
   );

   newVector.add_test(
      "New vector has size 0", [](auto & vec) {
         assert_equals(vec.size(), 0.0f);
      }
   );

   newVector.add_test(
      "Basic vector arithmetic", [](auto & vec) {
         vec = { 5.0f, -5.0f };
         assert_equals(vec.x,  5.0f, "Setting failed");
         assert_equals(vec.y, -5.0f, "Setting failed");

         vec = V2d<float> { -10.0f, 230.5f } + V2d<float> { 132.5f, -143.0f };
         assert_equals(vec.x,  122.5f, "Addition failed");
         assert_equals(vec.y,   87.5f, "Addition failed");

         vec = V2d<float> { 13.0f, 5.0f } - V2d<float> { 5.0f, 3.0f };
         assert_equals(vec.x, 8.0f, "Subtraction failed");
         assert_equals(vec.y, 2.0f, "Subtraction failed");

         vec = V2d<float> { 64.0f, -32.0f } * 2.5f;
         assert_equals(vec.x, 160.0f, "Multiplication failed");
         assert_equals(vec.y, -80.0f, "Multiplication failed");

         vec = V2d<float> { 160.0f, -80.0f } / 4.0f;
         assert_equals(vec.x,  40.0f, "Division failed");
         assert_equals(vec.y, -20.0f, "Division failed");
      }
   );

   newVector.add_test(
      "Setting arithmetic", [](auto & vec) {
         vec += { 145.0f, -90.0f };
         assert_equals(vec.x, 145.0f, "Addition failed");
         assert_equals(vec.y, -90.0f, "Addition failed");

         vec -= { 55.0f, -135.0f };
         assert_equals(vec.x, 90.0f, "Subtraction failed");
         assert_equals(vec.y, 45.0f, "Subtraction failed");

         vec /= 3.0f;
         assert_equals(vec.x, 30.0f, "Division failed");
         assert_equals(vec.y, 15.0f, "Division failed");

         vec *= 3.0f;
         assert_equals(vec.x, 90.0f, "Multiplication failed");
         assert_equals(vec.y, 45.0f, "Multiplication failed");
      }
   );

   newVector.add_test(
      "Vector conversion from int->float", [](auto & vec) {
         V2d<int> intVec { 13023, -132 };

         vec = intVec;

         assert_equals(vec.x, 13023.0f);
         assert_equals(vec.y, -132.0f);
      }
   );
   
   newVector.add_test(
      "Vector convert from float->int->float", [](auto & vec) {
         V2d<float> vf { -32.0f, 70.0f };
         V2d<int>   vi = vf;
         V2d<float> vitof = vi;

         assert_equals(vf.x, vitof.x);
         assert_equals(vf.y, vitof.y);
      }
   );

   newVector.add_test(
      "Size of vec with x and y is equal to sqrt(x * x + y * y)", [](auto & vec) {
         vec = { 3.0f, 4.0f };
         assert_equals(vec.size(), 5.0f);
      }
   );

   newVector.add_test(
      "size_sq gives squared .size()", [](auto & vec) {
         vec = { -12.0f, 15.0f };
         float sz = vec.size();
         float szsq = vec.size_sq();
         float threshold = 0.0001f;
         assert_true(szsq > sz * sz - threshold);
         assert_true(szsq < sz * sz + threshold);
      }
   );

   newVector.add_test(
      "Check is bound", [](auto & vec) {
         vec = { -100.0f, 20.0f };

         assert_true(vec.is_bound({ -101, 19 }, { -99, 20 }));
         assert_false(vec.is_bound({0, 0}, {-10, -10}));
         assert_false(vec.is_bound({-50.0f, 15.0f}, {-25.0f, 25.0f}));
      }
   );

   newVector.add_test(
      "Normalize -> has size 1", [](auto & vec) {
         vec = { -50.0f, -20.0f };
         vec.normalize();
         assert_equals(vec.size(), 1.0f);
         V2d<float> normalizedVec = vec.normal();
         assert_equals(normalizedVec.size(), 1.0f);
      }
   );

   newVector.add_test(
      "Rotate 45 degrees lends correct angle", [](auto & vec) {
         vec = { 5.0f, 0.0f };
         V2d<float> rotated45 = { 5.0f, 5.0f };

         vec.rotate(quarterPi);
         assert_equals(vec.angle(), rotated45.angle());
      }
   );

   newVector.add_test(
      "Setting size x leads to vector with size x", [](auto & vec) {
         vec = { 42.0f, -23.0f };
         vec.size(500.0f);
         assert_approx(vec.size(), 500.0f);
      }
   );

   newVector.add_test(
      "dot product", [](auto & vec) {
         vec = { 42.0f, 69.0f };
         V2d<float> vec2 { 12.0f, 67.0f };
         assert_equals(dot_prod(vec, vec2), 12.0f * 42.0f + 69.0f * 67.0f);
      }
   );

   newVector.add_test<V2d<float>::NoAngle>(
      "angle of vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         vec.angle(20.0f);
      }
   );

   newVector.add_test<fbg::DivisionByZero>(
      "normalizing vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         vec.normalize();
      }
   );

   newVector.add_test<fbg::DivisionByZero>(
      "normal of vector with size 0 throws", [](auto & vec) {
         assert_equals(vec.size(), 0);
         V2d<float> n = vec.normal();
      }
   );

   newVector.add_test(
      "limiting vectors size when size is greater than limit", [](auto & vec) {
         vec = { 100.0f, -12.0f };
         vec.limit(50.0f);
         assert_approx(vec.size(), 50.0f);

         vec = { 23.0f, 10.0f };
         float oldsz = vec.size();
         vec.limit(50.0f);
         assert_equals(vec.size(), oldsz);
      }
   );

   newVector.add_test(
      "rotating vector leads to correct angle", [](auto & vec) {
         vec = { 10.0f, 10.0f };
         float oldang = vec.angle();
         vec.rotate(pi);
         assert_approx(vec.angle() - pi, oldang);
      }
   );

   newVector.add_test<V2d<float>::NoAngle>(
      "angle between vectors throws when size == 0", [](auto & vec) {
         V2d<float> a { 1.0f, 1.0f };
         angle_between(a, vec);
      }
   );

   newVector.run();

   std::cout << newVector << '\n';

}

void alpha_composite_test()
{
   MultiTest acTest { "alpha compositing algorithms" };

   acTest.add_test(
      "Single color over single color", []() {
         Rgba under { 128, 140, 78, 100 };
         Rgba over  { 40, 130, 240, 150 };

         alpha_composite1(&under, &over);

         assert_equals(under.r, (u_int8_t)  76, "red invalid");
         assert_equals(under.g, (u_int8_t) 134, "green invalid");
         assert_equals(under.b, (u_int8_t) 173, "blue invalid");
         assert_equals(under.a, (u_int8_t) 129, "alpha invalid");
      }
   );

   acTest.add_test(
      "Array of color over array of color (length 8)", []() {
         Rgba under[8];
         Rgba oldUnder[8];
         Rgba over [8];

         srand(clock());
         for (size_t i = 0; i < 8; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over [i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_composite8(under, over);

         for (size_t i = 0; i < 8; i++) {
            assert_equals(under[i].r, ((uint8_t) ((over[i].r * over[i].a + oldUnder[i].r * (255 - over[i].a)) / 256)), "red invalid, index: " + std::to_string(i));
            assert_equals(under[i].g, ((uint8_t) ((over[i].g * over[i].a + oldUnder[i].g * (255 - over[i].a)) / 256)), "green invalid, index: " + std::to_string(i));
            assert_equals(under[i].b, ((uint8_t) ((over[i].b * over[i].a + oldUnder[i].b * (255 - over[i].a)) / 256)), "blue invalid, index: " + std::to_string(i));
            assert_equals(under[i].a, ((uint8_t) ((over[i].a * over[i].a + oldUnder[i].a * (255 - over[i].a)) / 256)), "alpha invalid, index: " + std::to_string(i));
         }
      }
   );

   acTest.add_test(
      "Array of color over array of color (length n)", []() {
         int n = rand() % 20000;

         Rgba * under = new Rgba[n];
         Rgba * oldUnder = new Rgba[n];
         Rgba * over = new Rgba[n];

         for (size_t i = 0; i < n; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over [i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_compositeN(under, over, n);

         for (size_t i = 0; i < n; i++) {
            // within margin 1?
            assert_approx(under[i].r, 
               ((uint8_t) ((over[i].r * over[i].a + oldUnder[i].r * (255 - over[i].a)) / 256)), 
               "red invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].g, 
               ((uint8_t) ((over[i].g * over[i].a + oldUnder[i].g * (255 - over[i].a)) / 256)), 
               "green invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].b, 
               ((uint8_t) ((over[i].b * over[i].a + oldUnder[i].b * (255 - over[i].a)) / 256)), 
               "blue invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].a, 
               ((uint8_t) ((over[i].a * over[i].a + oldUnder[i].a * (255 - over[i].a)) / 256)), 
               "alpha invalid, index: " + std::to_string(i), 1);
         }
      }
   );

   acTest.add_test(
      "One color over array of color (length n)", []() {
         int n = rand() % 20000;

         Rgba * under = new Rgba[n];
         Rgba * oldUnder = new Rgba[n];
         Rgba over;

         for (size_t i = 0; i < n; i++) {

            under[i] = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                         static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };

            oldUnder[i] = under[i];

            over = { static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U), 
                     static_cast<u_int8_t>(rand() % 255U), static_cast<u_int8_t>(rand() % 255U) };
         }

         alpha_compositeNC(under, &over, n);

         for (size_t i = 0; i < n; i++) {
            // within margin 1?
            assert_approx(under[i].r, 
               ((uint8_t) ((over.r * over.a + oldUnder[i].r * (255 - over.a)) / 256)), 
               "red invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].g, 
               ((uint8_t) ((over.g * over.a + oldUnder[i].g * (255 - over.a)) / 256)), 
               "green invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].b, 
               ((uint8_t) ((over.b * over.a + oldUnder[i].b * (255 - over.a)) / 256)), 
               "blue invalid, index: " + std::to_string(i), 1);

            assert_approx(under[i].a, 
               ((uint8_t) ((over.a * over.a + oldUnder[i].a * (255 - over.a)) / 256)), 
               "alpha invalid, index: " + std::to_string(i), 1);
         }
      }
   );

   acTest.run();

   std::cout << acTest << '\n';
}

void frame_test()
{
   MultiTest<Frame> frameTest { "Test of frame class" };
   srand(clock());

   frameTest.add_constructor("Constructor with size n", [](auto * & frame) {
      int x = rand() % 1400 + 10;
      int y = rand() % 1400 + 10;
      frame = new Frame { x, y };
   });

   frameTest.add_constructor("Copy constructor", [](auto * & frame) {
      Frame cp { 200, 300 };
      frame = new Frame { cp };
   });

   frameTest.add_constructor("Move constructor", [](auto * & frame) {
      Frame cp { 200, 300 };
      frame = new Frame { std::move(cp) };
   });

   frameTest.add_test<Frame::OutOfRange>(
      "getting pixel -1, -1 throws out of range", [](auto & frame) {
         frame.get(-1, -1);
      }
   );

   frameTest.add_test<Frame::OutOfRange>(
      "getting pixel (w, h) throws out of range", [](auto & frame) {
         frame.get(frame.w, frame.h);
      }
   );

   frameTest.add_test(
      "New frame is black", [](auto & frame) {
         for (int y = 0; y < frame.h; y++)
            for (int x = 0; x < frame.w; x++) {
               Rgba col = frame.get(x, y); 
               assert_equals(col.r, 0, "Red not 0");
               assert_equals(col.g, 0, "Red not 0");
               assert_equals(col.b, 0, "Red not 0");
               assert_equals(col.a, 0, "Red not 0");
            }
      }
   );

   frameTest.add_test(
      "Setting pixel result in pixel having that color", [](auto & frame) {
         frame.set_pixel(5, 6, { 1, 2, 3, 255 });
         Rgba res { frame.get(5, 6) };
         assert_equals(res.r, 1, "red invalid");
         assert_equals(res.g, 2, "red invalid");
         assert_equals(res.b, 3, "red invalid");
         assert_equals(res.a, 255, "red invalid");
      }
   );

   frameTest.add_test(
      "Test of in_bound", [](auto & frame) {
         assert_true(frame.in_bound(0, 0));
         assert_false(frame.in_bound(frame.w, frame.h));
         assert_false(frame.in_bound(-1, -1));
      }
   );

   frameTest.run();
   std::cout << frameTest << '\n';
}

void shape_tests()
{
   MultiTest<Line> lineT { "Line class" };
   MultiTest<Rect> rectT { "Rectangle class" };
   MultiTest<Circle> circleT { "Circle class" };
   MultiTest<Triangle> triangleT { "Triangle class" };
   MultiTest<Polyline> polyShapeT { "polyline and base class (Shape) functionality" };
   MultiTest<Context, Circle, Rect, Triangle> contextT { "Context class" };

   lineT.add_constructor("Default constructor + set to()", [](auto * & line) {
      line = new Line { };
      line->to(20.0f, 10.0f);
   });

   lineT.add_constructor("Constructor with points", [](auto * & line) {
      line = new Line { V2d<float> { 0.0f, 0.0f }, V2d<float> { 20.0f, 10.0f }};
   });

   lineT.add_constructor("Constructor with x, y", [](auto * & line) {
      line = new Line { 0, 0, 20, 10 };
   });

   lineT.add_test(
      "from() sets from() (V2d<float>)", [](auto & line) {
         line.from(V2d<float> {32.0f, 132.0f});
         assert_equals(32.0f, line.from().x);
         assert_equals(132.0f, line.from().y);
      }
   );

   lineT.add_test(
      "from() sets from() (x, y)", [](auto & line) {
         line.from(32.0f, 132.0f);
         assert_equals(32.0f, line.from().x);
         assert_equals(132.0f, line.from().y);
      }
   );

   lineT.add_test(
      "to() sets to() (V2d<float>)", [](auto & line) {
         line.to(V2d<float> {32.0f, 132.0f});
         assert_equals(32.0f, line.to().x);
         assert_equals(132.0f, line.to().y);
      }
   );

   lineT.add_test(
      "to() sets to() (x, y)", [](auto & line) {
         line.to(32.0f, 132.0f);
         assert_equals(32.0f, line.to().x);
         assert_equals(132.0f, line.to().y);
      }
   );

   rectT.add_constructor(
      "Default constructor + width and height - corner", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CORNER;
         rect = new Rect { };
         rect->width(100.0f);
         rect->height(200.0f);
      }
   );

   rectT.add_constructor(
      "Default constructor + width and height - middle", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CENTER;
         rect = new Rect { };
         rect->width(100.0f);
         rect->height(200.0f);
      }
   );


   rectT.add_constructor(
      "Corner with (x, y)", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CORNER;
         rect = new Rect { 0.0f, 0.0f, 100.0f, 200.0f };
      }
   );

   rectT.add_constructor(
      "Middle with (x, y)", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CENTER;
         rect = new Rect { 0.0f, 0.0f, 100.0, 200.0f };
      }
   );

   rectT.add_constructor(
      "Corner with V2d<float>", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CORNER;
         V2d<float> p { 0.0f, 0.0f };
         rect = new Rect { p, 100.0f, 200.0f };
      }
   );

   rectT.add_constructor(
      "Middle with V2d<float>", [](auto * & rect) {
         Rect::MODE = Rect::DrawMode::CENTER;
         V2d<float> p { 0.0f, 0.0f };
         rect = new Rect { p, 100.0, 200.0f };
      }
   );

   rectT.add_test(
      "width and height are correct after construction", [](auto & rect) {
         assert_equals(rect.width() , 100.0f);
         assert_equals(rect.height(), 200.0f);
      }
   );

   rectT.add_test(
      "New rect has position (0, 0)", [](auto & rect) {
         assert_equals(rect.pos().x, 0.0f);
         assert_equals(rect.pos().y, 0.0f);
      }
   );

   rectT.add_test(
      "width() and height() set width() and height()", [](auto & rect) {
         rect.width(323023.0f);
         rect.height(132003.0f);
         assert_equals(rect.width(),  323023.0f);
         assert_equals(rect.height(), 132003.0f);
      }
   );

   rectT.add_test(
      "setting angle sets angle", [](auto & rect) {
         rect.angle(0.04f);
         assert_approx(rect.angle(), 0.04f);
      }
   );

   rectT.add_test(
      "rotating sets correct angle", [](auto & rect) {
         rect.angle(0.075f);
         rect.rotate(0.1001f);
         assert_approx(rect.angle(), 0.1751f);
      }
   );

   rectT.add_test<Rect::InvalidSize>(
      "Setting negative width throws", [](auto & rect) {
         rect.width(-1.05f);
      }
   );

   rectT.add_test<Rect::InvalidSize>(
      "Setting negative height throws", [](auto & rect) {
         rect.height(-1.05f);
      }
   );

   rectT.add_test(
      "position sets position and doesn't screw with angle", [](auto & rect) {
         rect.rotate(0.5f);
         float olda = 0.5f;
         rect.pos(1320.0f, -132.0f);
         assert_equals(rect.pos().x, 1320.0f);
         assert_equals(rect.pos().y, -132.0f);

         assert_approx(rect.angle(), olda);

         rect.pos(V2d<float> { -112.0f, 565.0f });
         assert_equals(rect.pos().x, -112.0f);
         assert_equals(rect.pos().y,  565.0f);

         assert_approx(rect.angle(), olda);
      }
   );

   rectT.add_test(
      "Rotating 360 does nothing", [](auto & rect) {
         rect.angle(0.0f);
         for (int i = 0; i < 100; i++)
            rect.rotate(twoPi);
         assert_approx(rect.angle(), 0.0f);
      }
   );

   circleT.add_constructor(
      "Default constructor + radius()", [](auto & circ) {
         circ = new Circle { };
         circ->radius(25.0f);
      }
   );

   circleT.add_constructor(
      "Point and radius", [](auto & circ) {
         circ = new Circle { V2d<float> { 0.0f, 0.0f }, 25.0f };
      }
   );

   circleT.add_constructor(
      "Coords and radius", [](auto & circ) {
         circ = new Circle { 0.0f, 0.0f, 25.0f };
      }
   );

   circleT.add_test(
      "New circle has position (0, 0)", [](auto & circ) {
         assert_equals(circ.pos().x, 0.0f);
         assert_equals(circ.pos().y, 0.0f);
      }
   );

   circleT.add_test(
      "New circle has correct radius", [](auto & circ) {
         assert_equals(circ.radius(), 25.0f);
      }
   );

   circleT.add_test(
      "Setting pos() sets pos()", [](auto & circ) {
         circ.pos(25.0f, -132.0f);
         assert_equals(circ.pos().x,   25.0f);
         assert_equals(circ.pos().y, -132.0f);

         circ.pos(V2d<float> { 323.0f, 111.0f });
         assert_equals(circ.pos().x, 323.0f);
         assert_equals(circ.pos().y, 111.0f);
      }
   );

   circleT.add_test<Circle::InvalidRadius>(
      "Setting negative radius throws error", [](auto & circ) {
         circ.radius(-0.01f);
      }
   );

   triangleT.add_constructor(
      "Default constructor, setting points", [](auto * & tri) {
         tri = new Triangle;
         tri->point(0,  10.0f,  20.0f);
         tri->point(1, 110.0f,  50.0f);
         tri->point(2, -20.0f, 100.0f);
      } 
   );

   triangleT.add_constructor(
      "Points in constructor", [](auto * & tri) {
         tri = new Triangle { { 10.0f, 20.0f }, { 110.0f, 50.0f }, { -20.0f, 100.0f }};
      }
   );

   triangleT.add_test(
      "New triangle has correct points", [](auto & tri) {
         assert_equals(tri.point(0).x,  10.0f);
         assert_equals(tri.point(0).y,  20.0f);
         assert_equals(tri.point(1).x, 110.0f);
         assert_equals(tri.point(1).y,  50.0f);
         assert_equals(tri.point(2).x, -20.0f);
         assert_equals(tri.point(2).y, 100.0f);
      }
   );

   triangleT.add_test(
      "Setting point sets point (x, y)", [](auto & tri) {
         tri.point(1, 0.002f, -11.0f);
         assert_equals(tri.point(1).x, 0.002f);
         assert_equals(tri.point(1).y, -11.0f);
      }
   );

   triangleT.add_test(
      "Setting point sets point V2d<float>", [](auto & tri) {
         tri.point(2, { 0.252f, -1323.0f});
         assert_equals(tri.point(2).x, 0.252f);
         assert_equals(tri.point(2).y, -1323.0f);
      }
   );

   polyShapeT.add_constructor(
      "Default constructor + setting points", [](auto * & pl) {
         pl = new Polyline;
         pl->vertex(13.0f,  -23.0f);
         pl->vertex(100.0f, -60.0f);
         pl->vertex(-200.0f, 10.0f);
         pl->vertex(123.0f,  15.0f);
      }
   );

   polyShapeT.add_constructor(
      "Initializer list", [](auto * & pl) {
         pl = new Polyline { 
            {   13.0f, -23.0f },
            {  100.0f, -60.0f },
            { -200.0f,  10.0f },
            {  123.0f,  15.0f }
         };
      }
   );

   polyShapeT.add_test(
      "Vertices are set correctly after construction", [](auto & pl) {
         assert_equals(pl.get_vertex(0).x,   13.0f, "index 0.x wrong");
         assert_equals(pl.get_vertex(0).y,  -23.0f, "index 0.y wrong");
         assert_equals(pl.get_vertex(1).x,  100.0f, "index 1.x wrong");
         assert_equals(pl.get_vertex(1).y,  -60.0f, "index 1.y wrong");
         assert_equals(pl.get_vertex(2).x, -200.0f, "index 2.x wrong");
         assert_equals(pl.get_vertex(2).y,   10.0f, "index 2.y wrong");
         assert_equals(pl.get_vertex(3).x,  123.0f, "index 3.x wrong");
         assert_equals(pl.get_vertex(3).y,   15.0f, "index 3.y wrong");
      }
   );
   
   polyShapeT.add_test(
      "Setting vertices", [](auto & pl) {
         pl.set_vertex(2, {100.0f, 32.0f});
         assert_equals(pl.get_vertex(2).x, 100.0f);
         assert_equals(pl.get_vertex(2).y,  32.0f);
      
         pl.set_vertex(1, 10.0f, 332.0f);
         assert_equals(pl.get_vertex(1).x, 10.0f);
         assert_equals(pl.get_vertex(1).y, 332.0f);
      }
   );

   polyShapeT.add_test(
      "Adding vertices works and doesn't change existing vertices", [](auto & pl) {
         V2d<float> before2 { pl.get_vertex(2) }; 

         pl.vertex(3.0f, 13.0f);
         assert_equals(pl.get_vertex(4).x,  3.0f);
         assert_equals(pl.get_vertex(4).y, 13.0f);

         assert_equals(pl.get_vertex(2).x, before2.x);
         assert_equals(pl.get_vertex(2).y, before2.y);
      }
   );

   polyShapeT.add_test(
      "Shape: rotating around point rotates shape (around V2d<float>)", [](auto & pl) {
         V2d<float> diff = pl.get_vertex(2) - V2d<float> {0, 0};
         diff.rotate(quarterPi); // compare to tested vector rotation

         pl.rotate(quarterPi, {0, 0});

         assert_approx(pl.get_vertex(2).x, diff.x);
         assert_approx(pl.get_vertex(2).y, diff.y);
      }
   );

   polyShapeT.add_test(
      "Shape: rotating around point rotates shape (around point 0)", [](auto & pl) {
         V2d<float> diff2 { pl.get_vertex(2) - pl.get_vertex(0) };
         pl.rotate(0.05f);
         diff2.rotate(0.05f);
         diff2 += pl.get_vertex(0);
         assert_approx(pl.get_vertex(2).x, diff2.x);
         assert_approx(pl.get_vertex(2).y, diff2.y);
      }
   );

   polyShapeT.add_test(
      "Shape: rotating around point rotates shape (around point x, y)", [](auto & pl) {
         V2d<float> diff3 { pl.get_vertex(2) - V2d<float> { 23.0f, 13.0f }};
         pl.rotate(0.32f, 23.0f, 13.0f);
         diff3.rotate(0.32f);
         diff3 += V2d<float> { 23.0f, 13.0f };
         assert_approx(pl.get_vertex(2).x, diff3.x);
         assert_approx(pl.get_vertex(2).y, diff3.y);
      }
   );

   polyShapeT.add_test(
      "Shape: moving V2d<float>", [](auto & pl) {
         V2d<float> old0 { pl.get_vertex(0) };
         V2d<float> old1 { pl.get_vertex(1) };
         V2d<float> old2 { pl.get_vertex(2) };
         V2d<float> old3 { pl.get_vertex(3) };

         pl.move(V2d<float> { 23.0f, 56.0f });

         assert_equals(pl.get_vertex(0).x, old0.x + 23.0f);
         assert_equals(pl.get_vertex(0).y, old0.y + 56.0f);
         assert_equals(pl.get_vertex(1).x, old1.x + 23.0f);
         assert_equals(pl.get_vertex(1).y, old1.y + 56.0f);
         assert_equals(pl.get_vertex(2).x, old2.x + 23.0f);
         assert_equals(pl.get_vertex(2).y, old2.y + 56.0f);
         assert_equals(pl.get_vertex(3).x, old3.x + 23.0f);
         assert_equals(pl.get_vertex(3).y, old3.y + 56.0f);  
      }
   );

   polyShapeT.add_test(
      "Shape: moving (x, y)", [](auto & pl) {
         V2d<float> old0 { pl.get_vertex(0) };
         V2d<float> old1 { pl.get_vertex(1) };
         V2d<float> old2 { pl.get_vertex(2) };
         V2d<float> old3 { pl.get_vertex(3) };

         pl.move(V2d<float> { -51.5f, 23.01f });

         assert_equals(pl.get_vertex(0).x, old0.x + -51.5f);
         assert_equals(pl.get_vertex(0).y, old0.y + 23.01f);
         assert_equals(pl.get_vertex(1).x, old1.x + -51.5f);
         assert_equals(pl.get_vertex(1).y, old1.y + 23.01f);
         assert_equals(pl.get_vertex(2).x, old2.x + -51.5f);
         assert_equals(pl.get_vertex(2).y, old2.y + 23.01f);
         assert_equals(pl.get_vertex(3).x, old3.x + -51.5f);
         assert_equals(pl.get_vertex(3).y, old3.y + 23.01f);
      }
   );

   polyShapeT.add_test(
      "Shape: Color (stroke)", [](auto & pl) {
         Rgba col1 { 123,  56,  19,  14 };
         Rgba col2 { 86,  240,  79, 140 };
         Rgba col3 { 11,  198,  23, 255 };
         Rgba col4 { 140, 140, 140,  20 };
         Rgba col5 { 230, 230, 230, 255 };

         pl.stroke(col1);
         assert_equals(col1.r, pl.stroke().r);
         assert_equals(col1.g, pl.stroke().g);
         assert_equals(col1.b, pl.stroke().b);
         assert_equals(col1.a, pl.stroke().a);

         pl.stroke(86, 240, 79, 140);
         assert_equals(col2.r, pl.stroke().r);
         assert_equals(col2.g, pl.stroke().g);
         assert_equals(col2.b, pl.stroke().b);
         assert_equals(col2.a, pl.stroke().a);
         
         pl.stroke(11,  198, 23);
         assert_equals(col3.r, pl.stroke().r);
         assert_equals(col3.g, pl.stroke().g);
         assert_equals(col3.b, pl.stroke().b);
         assert_equals(col3.a, pl.stroke().a);
         
         pl.stroke(140, 20);
         assert_equals(col4.r, pl.stroke().r);
         assert_equals(col4.g, pl.stroke().g);
         assert_equals(col4.b, pl.stroke().b);
         assert_equals(col4.a, pl.stroke().a);
         
         pl.stroke(230);
         assert_equals(col5.r, pl.stroke().r);
         assert_equals(col5.g, pl.stroke().g);
         assert_equals(col5.b, pl.stroke().b);
         assert_equals(col5.a, pl.stroke().a);
      }
   );

   polyShapeT.add_test(
      "Shape: Color (fill)", [](auto & pl) {
         Rgba col1 { 123,  56,  19,  14 };
         Rgba col2 { 86,  240,  79, 140 };
         Rgba col3 { 11,  198,  23, 255 };
         Rgba col4 { 140, 140, 140,  20 };
         Rgba col5 { 230, 230, 230, 255 };

         pl.fill(col1);
         assert_equals(col1.r, pl.fill().r);
         assert_equals(col1.g, pl.fill().g);
         assert_equals(col1.b, pl.fill().b);
         assert_equals(col1.a, pl.fill().a);

         pl.fill(86, 240, 79, 140);
         assert_equals(col2.r, pl.fill().r);
         assert_equals(col2.g, pl.fill().g);
         assert_equals(col2.b, pl.fill().b);
         assert_equals(col2.a, pl.fill().a);
         
         pl.fill(11,  198, 23);
         assert_equals(col3.r, pl.fill().r);
         assert_equals(col3.g, pl.fill().g);
         assert_equals(col3.b, pl.fill().b);
         assert_equals(col3.a, pl.fill().a);
         
         pl.fill(140, 20);
         assert_equals(col4.r, pl.fill().r);
         assert_equals(col4.g, pl.fill().g);
         assert_equals(col4.b, pl.fill().b);
         assert_equals(col4.a, pl.fill().a);
         
         pl.fill(230);
         assert_equals(col5.r, pl.fill().r);
         assert_equals(col5.g, pl.fill().g);
         assert_equals(col5.b, pl.fill().b);
         assert_equals(col5.a, pl.fill().a);
      }
   );

   auto constructShapes = [](Circle * & circ, Rect * & rect, Triangle * & tri) {
      circ = new Circle   { 25.0f, -100.0f, 40.0f };
      rect = new Rect     { 250.0f, 10.0f, 40.0f, 80.0f };
      tri  = new Triangle { { 120.0f, 100.0f }, { 40.0f, -80.0f }, { -10.0f, 15.0f }};
   };

   contextT.add_constructor(
      "Default context, attach invidually, set origin point", [&](auto * & context, auto * & circ, auto * & rect, auto * & tri) {
         context = new Context;

         context->origin(450.0f, 224.0f);
         constructShapes(circ, rect, tri);
         context->attach(*circ, *rect);
         context->attach(*tri);
      }
   );

   contextT.add_constructor(
      "Context with origin point (V2d<float>)", [&](auto * & context, auto * & circ, auto * &rect, auto * & tri) {
         constructShapes(circ, rect, tri);
         context = new Context { 450.0f, 224.0f };
         context->attach(*circ, *rect, *tri);
      }
   );

   contextT.add_constructor(
      "Context with origin point (x, y)", [&](auto * & context, auto * & circ, auto * &rect, auto * & tri) {
         constructShapes(circ, rect, tri);
         context = new Context { V2d<float> { 450.0f, 224.0f } };
         context->attach(*circ);
         context->attach(*rect);
         context->attach(*tri);
      }
   );

   contextT.add_test(
      "Constructed context has correct shapes and origin point", [](auto & context, auto & circ, auto & rect, auto & tri) {
         assert_equals(context.origin().x, 450.0f, "context wrong position");
         assert_equals(context.origin().y, 224.0f, "context wrong position");

         std::vector<Shape *> shapes { context.getShapes() };
         assert_equals(*shapes[0], circ);
         assert_equals(*shapes[1], rect);
         assert_equals(*shapes[2], tri);

         assert_equals(circ.pos().x,   25.0f, "circle wrong position");
         assert_equals(circ.pos().y, -100.0f, "circle wrong position");
         assert_equals(circ.radius(), 40.0f, "circle wrong radius"); 
         // should assure correctness
      }
   );

   contextT.add_test(
      "Attaching new shape adds shape to context", [](auto & context, auto & circ, auto & rect, auto & tri) {
         Line l { 100.0f, 100.0f, 200.0f, 200.0f };
         context.attach(l);
         assert_equals(*context.getShapes().back(), l);
      }
   );

   contextT.add_test(
      "Detaching all makes context empty", [](auto & context, auto & circ, auto & rect, auto & tri) {
         context.detach(circ);
         context.detach(rect);
         context.detach(tri);

         assert_true(context.empty(), "Context isn't empty");
      }
   );

   contextT.add_test<Context::UndrawableShape>(
      "Attaching undrawable shape throws error", [](auto & context, auto & circ, auto & rect, auto & tri) {
         Polyline pl; // no point -> invalid

         context.attach(pl);
      }
   );

   contextT.add_test<Context::SelfAttach>(
      "Attaching to self throws error", [](auto & context, auto & circ, auto & rect, auto & tri) {
         context.attach(context);
      }
   );

   lineT.run();
   rectT.run();
   circleT.run();
   triangleT.run();
   polyShapeT.run();
   contextT.run();

   std::cout << lineT      << '\n' 
             << rectT      << '\n'
             << circleT    << '\n'
             << triangleT  << '\n'
             << polyShapeT << '\n'
             << contextT   << '\n';
}